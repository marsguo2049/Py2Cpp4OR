#include "py2cpp4or/gvns/core.h"

#include <cmath>
#include <iomanip>
#include <locale>
#include <sstream>
#include <utility>

namespace py2cpp4or {
namespace gvns {

WorkUnitSum saturating_add_work_units(
    const std::size_t current, const std::size_t additional) noexcept {
    const std::size_t maximum = std::numeric_limits<std::size_t>::max();
    if (additional > maximum - current) {
        return WorkUnitSum{maximum, true};
    }
    return WorkUnitSum{current + additional, false};
}

const char* to_string(const NeighborhoodStatus status) noexcept {
    switch (status) {
        case NeighborhoodStatus::improved:
            return "IMPROVED";
        case NeighborhoodStatus::exhausted:
            return "EXHAUSTED";
        case NeighborhoodStatus::interrupted:
            return "INTERRUPTED";
        case NeighborhoodStatus::disabled:
            return "DISABLED";
    }
    return "UNKNOWN";
}

const char* to_string(const SearchTermination termination) noexcept {
    switch (termination) {
        case SearchTermination::local_optimum:
            return "LOCAL_OPTIMUM";
        case SearchTermination::interrupted:
            return "INTERRUPTED";
        case SearchTermination::no_enabled_neighborhoods:
            return "NO_ENABLED_NEIGHBORHOODS";
    }
    return "UNKNOWN";
}

const char* to_string(const TraceEventKind kind) noexcept {
    switch (kind) {
        case TraceEventKind::neighborhood_result:
            return "neighborhood_result";
        case TraceEventKind::search_stopped:
            return "search_stopped";
        case TraceEventKind::search_completed:
            return "search_completed";
    }
    return "unknown";
}

StopDecision NeverStopPolicy::evaluate(const SearchProgress&,
                                       const Clock&) const {
    return StopDecision{};
}

DeadlineStopPolicy::DeadlineStopPolicy(const std::uint64_t deadline_ticks,
                                       std::string reason)
    : deadline_ticks_{deadline_ticks}, reason_{std::move(reason)} {
    if (reason_.empty()) {
        throw std::invalid_argument{"stop reason must not be empty"};
    }
}

StopDecision DeadlineStopPolicy::evaluate(const SearchProgress&,
                                          const Clock& clock) const {
    if (clock.now_ticks() >= deadline_ticks_) {
        return StopDecision{true, reason_};
    }
    return StopDecision{};
}

FixedWorkStopPolicy::FixedWorkStopPolicy(const std::size_t work_limit,
                                         std::string reason)
    : work_limit_{work_limit}, reason_{std::move(reason)} {
    if (reason_.empty()) {
        throw std::invalid_argument{"stop reason must not be empty"};
    }
}

StopDecision FixedWorkStopPolicy::evaluate(const SearchProgress& progress,
                                           const Clock&) const {
    if (progress.work_units >= work_limit_) {
        return StopDecision{true, reason_};
    }
    return StopDecision{};
}

void NullObserver::on_event(const TraceEvent&) {}

void VectorTraceObserver::on_event(const TraceEvent& event) {
    events_.push_back(event);
}

const std::vector<TraceEvent>& VectorTraceObserver::events() const noexcept {
    return events_;
}

std::vector<std::string> VectorTraceObserver::golden_trace() const {
    std::vector<std::string> trace;
    trace.reserve(events_.size());
    for (const auto& event : events_) {
        trace.push_back(format_trace_event(event));
    }
    return trace;
}

std::string format_trace_event(const TraceEvent& event) {
    std::ostringstream output;
    output.imbue(std::locale::classic());
    output << event.sequence << '|' << to_string(event.kind) << '|'
           << event.run_id << '|'
           << (event.neighborhood_id.empty() ? "-" : event.neighborhood_id)
           << '|';
    if (event.status) {
        output << to_string(*event.status);
    } else {
        output << '-';
    }
    output << '|' << std::fixed << std::setprecision(6)
           << event.objective_before << '|' << event.objective_after << '|'
           << (event.accepted ? 1 : 0) << '|' << event.work_units << '|'
           << event.rng_draws << '|' << event.clock_ticks << '|'
           << (event.detail.empty() ? "-" : event.detail);
    return output.str();
}

StrictMinimizationAcceptance::StrictMinimizationAcceptance(
    const double tolerance)
    : tolerance_{tolerance} {
    if (!std::isfinite(tolerance_) || tolerance_ < 0.0) {
        throw std::invalid_argument{
            "acceptance tolerance must be finite and non-negative"};
    }
}

bool StrictMinimizationAcceptance::accepts(
    const double incumbent_objective, const double candidate_objective) const {
    if (!std::isfinite(incumbent_objective) ||
        !std::isfinite(candidate_objective)) {
        throw std::invalid_argument{"objectives must be finite"};
    }
    return candidate_objective < incumbent_objective - tolerance_;
}

SequentialNeighborhoodChange::SequentialNeighborhoodChange(
    std::vector<int> levels)
    : levels_{std::move(levels)} {
    if (levels_.empty()) {
        throw std::invalid_argument{"neighborhood levels must not be empty"};
    }
    for (const int level : levels_) {
        if (level <= 0) {
            throw std::invalid_argument{
                "neighborhood levels must be positive"};
        }
    }
}

int SequentialNeighborhoodChange::current_level() const noexcept {
    return levels_[position_];
}

std::size_t SequentialNeighborhoodChange::completed_cycles() const noexcept {
    return completed_cycles_;
}

void SequentialNeighborhoodChange::on_rejected() noexcept {
    ++position_;
    if (position_ == levels_.size()) {
        position_ = 0U;
        ++completed_cycles_;
    }
}

void SequentialNeighborhoodChange::on_accepted() noexcept {
    position_ = 0U;
}

}  // namespace gvns
}  // namespace py2cpp4or
