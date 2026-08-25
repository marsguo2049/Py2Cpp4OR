#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "py2cpp4or/gvns/compat.h"

namespace py2cpp4or {
namespace gvns {

template <typename Value>
class OptionalValue {
public:
    OptionalValue() = default;
    OptionalValue(Value value) : present_{true}, value_{std::move(value)} {}

    PY2CPP4OR_NODISCARD explicit operator bool() const noexcept {
        return present_;
    }

    PY2CPP4OR_NODISCARD const Value& value() const {
        if (!present_) {
            throw std::logic_error{"optional value is not set"};
        }
        return value_;
    }

    PY2CPP4OR_NODISCARD const Value& operator*() const { return value(); }

private:
    bool present_{};
    Value value_{};
};

enum class NeighborhoodStatus {
    improved,
    exhausted,
    interrupted,
    disabled,
};

enum class SearchTermination {
    local_optimum,
    interrupted,
    no_enabled_neighborhoods,
};

enum class TraceEventKind {
    neighborhood_result,
    search_stopped,
    search_completed,
};

PY2CPP4OR_NODISCARD const char* to_string(
    NeighborhoodStatus status) noexcept;
PY2CPP4OR_NODISCARD const char* to_string(
    SearchTermination termination) noexcept;
PY2CPP4OR_NODISCARD const char* to_string(TraceEventKind kind) noexcept;

class Clock {
public:
    virtual ~Clock() = default;
    PY2CPP4OR_NODISCARD virtual std::uint64_t now_ticks() const noexcept = 0;
};

class RandomSource {
public:
    virtual ~RandomSource() = default;
    virtual std::uint64_t next_u64() = 0;
    virtual std::size_t uniform_index(std::size_t upper_exclusive) = 0;
    PY2CPP4OR_NODISCARD virtual std::size_t draws_consumed()
        const noexcept = 0;
};

struct SearchProgress {
    std::size_t registry_visits{};
    std::size_t neighborhood_calls{};
    std::size_t accepted_improvements{};
    std::size_t work_units{};
    std::size_t cap_hits{};
};

struct StopDecision {
    bool stop{};
    std::string reason;
};

struct WorkUnitSum {
    std::size_t value{};
    bool overflowed{};
};

PY2CPP4OR_NODISCARD WorkUnitSum saturating_add_work_units(
    std::size_t current, std::size_t additional) noexcept;

class StopPolicy {
public:
    virtual ~StopPolicy() = default;
    PY2CPP4OR_NODISCARD virtual StopDecision evaluate(
        const SearchProgress& progress, const Clock& clock) const = 0;
};

class NeverStopPolicy final : public StopPolicy {
public:
    PY2CPP4OR_NODISCARD StopDecision evaluate(
        const SearchProgress& progress, const Clock& clock) const override;
};

class DeadlineStopPolicy final : public StopPolicy {
public:
    explicit DeadlineStopPolicy(std::uint64_t deadline_ticks,
                                std::string reason = "deadline");

    PY2CPP4OR_NODISCARD StopDecision evaluate(
        const SearchProgress& progress, const Clock& clock) const override;

private:
    std::uint64_t deadline_ticks_;
    std::string reason_;
};

// Deadline contract: VndEngine checks before each neighborhood call. A call
// started before the deadline is atomic and may finish; its completed outcome
// is processed, then no later call may start. A neighborhood that checks the
// policy during its call must return INTERRUPTED, never EXHAUSTED.

// Fixed work is an observer-independent test/replay budget. A neighborhood call
// is atomic, so the accumulated count may pass the limit before the next check.
class FixedWorkStopPolicy final : public StopPolicy {
public:
    explicit FixedWorkStopPolicy(std::size_t work_limit,
                                 std::string reason = "fixed-work-limit");

    PY2CPP4OR_NODISCARD StopDecision evaluate(
        const SearchProgress& progress, const Clock& clock) const override;

private:
    std::size_t work_limit_;
    std::string reason_;
};

struct TraceEvent {
    std::size_t sequence{};
    TraceEventKind kind{TraceEventKind::neighborhood_result};
    std::string profile_id;
    std::string neighborhood_id;
    OptionalValue<NeighborhoodStatus> status;
    double objective_before{};
    double objective_after{};
    bool accepted{};
    std::size_t work_units{};
    std::size_t rng_draws{};
    std::uint64_t clock_ticks{};
    std::string detail;
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void on_event(const TraceEvent& event) = 0;
};

class NullObserver final : public Observer {
public:
    void on_event(const TraceEvent& event) override;
};

class VectorTraceObserver final : public Observer {
public:
    void on_event(const TraceEvent& event) override;

    PY2CPP4OR_NODISCARD const std::vector<TraceEvent>& events() const noexcept;
    PY2CPP4OR_NODISCARD std::vector<std::string> golden_trace() const;

private:
    std::vector<TraceEvent> events_;
};

PY2CPP4OR_NODISCARD std::string format_trace_event(const TraceEvent& event);

class StrictImprovementAcceptance {
public:
    explicit StrictImprovementAcceptance(double tolerance);

    PY2CPP4OR_NODISCARD bool accepts(
        double incumbent_objective, double candidate_objective) const;

private:
    double tolerance_;
};

class SequentialNeighborhoodChange {
public:
    explicit SequentialNeighborhoodChange(std::vector<int> strengths);

    PY2CPP4OR_NODISCARD int current_strength() const noexcept;
    PY2CPP4OR_NODISCARD std::size_t completed_cycles() const noexcept;
    void on_rejected() noexcept;
    void on_accepted() noexcept;

private:
    std::vector<int> strengths_;
    std::size_t position_{};
    std::size_t completed_cycles_{};
};

struct NeighborhoodContext {
    RandomSource& rng;
    const Clock& clock;
    const SearchProgress& progress;
    const StopPolicy& stop_policy;

    PY2CPP4OR_NODISCARD StopDecision stop_after(
        const std::size_t additional_work = 0U) const {
        SearchProgress projected = progress;
        const WorkUnitSum sum =
            saturating_add_work_units(projected.work_units, additional_work);
        if (sum.overflowed) {
            return StopDecision{true, "work-units-overflow"};
        }
        projected.work_units = sum.value;
        return stop_policy.evaluate(projected, clock);
    }
};

template <typename Solution>
class NeighborhoodOutcome {
public:
    PY2CPP4OR_NODISCARD static NeighborhoodOutcome improved(
        Solution candidate, const std::size_t work_units = 0U) {
        return NeighborhoodOutcome{
            NeighborhoodStatus::improved,
            std::make_unique<Solution>(std::move(candidate)),
            work_units, false, {}};
    }

    PY2CPP4OR_NODISCARD static NeighborhoodOutcome exhausted(
        const std::size_t work_units = 0U) {
        return NeighborhoodOutcome{NeighborhoodStatus::exhausted, nullptr,
                                   work_units, false, {}};
    }

    PY2CPP4OR_NODISCARD static NeighborhoodOutcome interrupted(
        std::string detail, const std::size_t work_units = 0U,
        const bool cap_hit = false) {
        if (detail.empty()) {
            throw std::invalid_argument{"interrupted outcome needs a reason"};
        }
        return NeighborhoodOutcome{NeighborhoodStatus::interrupted,
                                   nullptr, work_units, cap_hit,
                                   std::move(detail)};
    }

    PY2CPP4OR_NODISCARD static NeighborhoodOutcome disabled(
        std::string detail = {}) {
        return NeighborhoodOutcome{NeighborhoodStatus::disabled, nullptr,
                                   0U, false, std::move(detail)};
    }

    PY2CPP4OR_NODISCARD NeighborhoodStatus status() const noexcept {
        return status_;
    }
    PY2CPP4OR_NODISCARD std::size_t work_units() const noexcept {
        return work_units_;
    }
    PY2CPP4OR_NODISCARD bool cap_hit() const noexcept { return cap_hit_; }
    PY2CPP4OR_NODISCARD const std::string& detail() const noexcept {
        return detail_;
    }

    PY2CPP4OR_NODISCARD Solution take_candidate() {
        if (status_ != NeighborhoodStatus::improved || !candidate_) {
            throw std::logic_error{"outcome does not contain a candidate"};
        }
        Solution candidate = std::move(*candidate_);
        candidate_.reset();
        return candidate;
    }

private:
    NeighborhoodOutcome(NeighborhoodStatus status,
                        std::unique_ptr<Solution> candidate,
                        std::size_t work_units, bool cap_hit,
                        std::string detail)
        : status_{status},
          candidate_{std::move(candidate)},
          work_units_{work_units},
          cap_hit_{cap_hit},
          detail_{std::move(detail)} {}

    NeighborhoodStatus status_;
    std::unique_ptr<Solution> candidate_;
    std::size_t work_units_;
    bool cap_hit_;
    std::string detail_;
};

template <typename Solution>
struct Neighborhood {
    using Search = std::function<NeighborhoodOutcome<Solution>(
        const Solution&, NeighborhoodContext&)>;

    std::string id;
    bool enabled{true};
    Search search;
};

template <typename Solution>
struct SearchResult {
    Solution solution;
    SearchTermination termination{SearchTermination::interrupted};
    bool registry_local_optimum{};
    SearchProgress progress;
    std::string termination_reason;
};

template <typename Solution>
class VndEngine {
public:
    using Objective = std::function<double(const Solution&)>;

    VndEngine(std::vector<Neighborhood<Solution>> registry,
              Objective objective,
              StrictImprovementAcceptance acceptance)
        : registry_{std::move(registry)},
          objective_{std::move(objective)},
          acceptance_{std::move(acceptance)} {
        if (registry_.empty()) {
            throw std::invalid_argument{"VND registry must not be empty"};
        }
        if (!objective_) {
            throw std::invalid_argument{"objective function must be provided"};
        }
        for (std::size_t left = 0; left < registry_.size(); ++left) {
            if (registry_[left].id.empty()) {
                throw std::invalid_argument{"neighborhood id must not be empty"};
            }
            for (std::size_t right = left + 1U; right < registry_.size();
                 ++right) {
                if (registry_[left].id == registry_[right].id) {
                    throw std::invalid_argument{"neighborhood ids must be unique"};
                }
            }
        }
    }

    PY2CPP4OR_NODISCARD SearchResult<Solution> run(
        Solution initial, std::string profile_id, RandomSource& rng,
        const Clock& clock, const StopPolicy& stop_policy,
        Observer& observer) const {
        if (profile_id.empty()) {
            throw std::invalid_argument{"profile id must not be empty"};
        }

        SearchProgress progress;
        std::size_t sequence{};
        std::size_t registry_position{};
        bool visited_exhaustible_neighborhood{};
        Solution incumbent = std::move(initial);

        const auto emit_terminal = [&](const TraceEventKind kind,
                                       const std::string& detail) {
            const double objective = objective_(incumbent);
            observer.on_event(TraceEvent{
                sequence, kind, profile_id, {}, {}, objective,
                objective, false, progress.work_units, rng.draws_consumed(),
                clock.now_ticks(), detail});
        };

        while (registry_position < registry_.size()) {
            const StopDecision decision = stop_policy.evaluate(progress, clock);
            if (decision.stop) {
                emit_terminal(TraceEventKind::search_stopped, decision.reason);
                return SearchResult<Solution>{
                    std::move(incumbent), SearchTermination::interrupted, false,
                    progress, decision.reason};
            }

            const auto& neighborhood = registry_[registry_position];
            ++progress.registry_visits;
            const double objective_before = objective_(incumbent);
            NeighborhoodOutcome<Solution> outcome =
                NeighborhoodOutcome<Solution>::disabled("profile-disabled");

            if (neighborhood.enabled) {
                if (!neighborhood.search) {
                    throw std::logic_error{"enabled neighborhood has no search"};
                }
                ++progress.neighborhood_calls;
                NeighborhoodContext context{rng, clock, progress, stop_policy};
                outcome = neighborhood.search(incumbent, context);
            }

            const WorkUnitSum work_sum = saturating_add_work_units(
                progress.work_units, outcome.work_units());
            progress.work_units = work_sum.value;
            if (outcome.cap_hit()) {
                ++progress.cap_hits;
            }

            bool accepted{};
            double objective_after = objective_before;
            if (work_sum.overflowed) {
                observer.on_event(TraceEvent{
                    sequence++, TraceEventKind::neighborhood_result, profile_id,
                    neighborhood.id, outcome.status(), objective_before,
                    objective_after, false, outcome.work_units(),
                    rng.draws_consumed(), clock.now_ticks(), outcome.detail()});
                emit_terminal(TraceEventKind::search_stopped,
                              "work-units-overflow");
                return SearchResult<Solution>{
                    std::move(incumbent), SearchTermination::interrupted, false,
                    progress, "work-units-overflow"};
            }
            if (outcome.status() == NeighborhoodStatus::improved) {
                Solution candidate = outcome.take_candidate();
                objective_after = objective_(candidate);
                if (!acceptance_.accepts(objective_before, objective_after)) {
                    throw std::logic_error{
                        "IMPROVED outcome failed strict acceptance"};
                }
                incumbent = std::move(candidate);
                ++progress.accepted_improvements;
                accepted = true;
                registry_position = 0U;
                visited_exhaustible_neighborhood = false;
            } else if (outcome.status() == NeighborhoodStatus::interrupted) {
                observer.on_event(TraceEvent{
                    sequence++, TraceEventKind::neighborhood_result, profile_id,
                    neighborhood.id, outcome.status(), objective_before,
                    objective_after, false, outcome.work_units(),
                    rng.draws_consumed(), clock.now_ticks(), outcome.detail()});
                emit_terminal(TraceEventKind::search_stopped, outcome.detail());
                return SearchResult<Solution>{
                    std::move(incumbent), SearchTermination::interrupted, false,
                    progress, outcome.detail()};
            } else {
                if (outcome.status() == NeighborhoodStatus::exhausted) {
                    visited_exhaustible_neighborhood = true;
                }
                ++registry_position;
            }

            observer.on_event(TraceEvent{
                sequence++, TraceEventKind::neighborhood_result, profile_id,
                neighborhood.id, outcome.status(), objective_before,
                objective_after, accepted, outcome.work_units(),
                rng.draws_consumed(), clock.now_ticks(), outcome.detail()});
        }

        const SearchTermination termination = visited_exhaustible_neighborhood
                                                  ? SearchTermination::local_optimum
                                                  : SearchTermination::no_enabled_neighborhoods;
        const std::string reason = visited_exhaustible_neighborhood
                                       ? "registry-local-optimum"
                                       : "no-enabled-neighborhoods";
        emit_terminal(TraceEventKind::search_completed, reason);
        return SearchResult<Solution>{
            std::move(incumbent), termination, visited_exhaustible_neighborhood,
            progress, reason};
    }

private:
    std::vector<Neighborhood<Solution>> registry_;
    Objective objective_;
    StrictImprovementAcceptance acceptance_;
};

}  // namespace gvns
}  // namespace py2cpp4or
