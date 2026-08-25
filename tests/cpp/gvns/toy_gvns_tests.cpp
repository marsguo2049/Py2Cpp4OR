#include "py2cpp4or/gvns/core.h"

#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using py2cpp4or::gvns::Clock;
using py2cpp4or::gvns::DeadlineStopPolicy;
using py2cpp4or::gvns::FixedWorkStopPolicy;
using py2cpp4or::gvns::Neighborhood;
using py2cpp4or::gvns::NeighborhoodContext;
using py2cpp4or::gvns::NeighborhoodOutcome;
using py2cpp4or::gvns::NeverStopPolicy;
using py2cpp4or::gvns::NullObserver;
using py2cpp4or::gvns::OptionalValue;
using py2cpp4or::gvns::RandomSource;
using py2cpp4or::gvns::SearchTermination;
using py2cpp4or::gvns::SequentialNeighborhoodChange;
using py2cpp4or::gvns::StrictMinimizationAcceptance;
using py2cpp4or::gvns::VectorTraceObserver;
using py2cpp4or::gvns::VndEngine;

struct ToySolution {
    int score{};
};

struct NonDefaultValue {
    explicit NonDefaultValue(const int input) : value{input} {}
    NonDefaultValue() = delete;

    int value;
};

class ToyClock final : public Clock {
public:
    explicit ToyClock(const std::uint64_t ticks = 0U) : ticks_{ticks} {}

    std::uint64_t now_ticks() const noexcept override { return ticks_; }
    void advance(const std::uint64_t ticks) noexcept { ticks_ += ticks; }

private:
    std::uint64_t ticks_{};
};

class ToyRandom final : public RandomSource {
public:
    explicit ToyRandom(std::vector<std::uint64_t> values)
        : values_{std::move(values)} {}

    std::uint64_t next_u64() override {
        if (values_.empty()) {
            throw std::logic_error{"toy random sequence must not be empty"};
        }
        const std::uint64_t value = values_[position_ % values_.size()];
        ++position_;
        ++draws_;
        return value;
    }

    std::size_t uniform_index(const std::size_t upper_exclusive) override {
        if (upper_exclusive == 0U) {
            throw std::invalid_argument{"upper bound must be positive"};
        }
        return static_cast<std::size_t>(next_u64() % upper_exclusive);
    }

    std::size_t draws_consumed() const noexcept override { return draws_; }

private:
    std::vector<std::uint64_t> values_;
    std::size_t position_{};
    std::size_t draws_{};
};

void require(const bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error{message};
    }
}

template <typename Actual, typename Expected>
void require_equal(const Actual& actual, const Expected& expected,
                   const std::string& message) {
    if (!(actual == expected)) {
        throw std::runtime_error{message};
    }
}

double objective(const ToySolution& solution) {
    return static_cast<double>(solution.score);
}

void test_improvements_restart_at_n1() {
    std::vector<std::string> calls;
    ToyClock clock;
    ToyRandom random{{11U}};
    NeverStopPolicy stop;
    VectorTraceObserver observer;

    std::vector<Neighborhood<ToySolution>> neighborhoods;
    neighborhoods.push_back(Neighborhood<ToySolution>{
        "N1-Increment", true,
        [&](const ToySolution& solution, NeighborhoodContext&) {
            calls.push_back("N1");
            clock.advance(1U);
            if (solution.score > 7) {
                return NeighborhoodOutcome<ToySolution>::improved(
                    ToySolution{7}, 1U);
            }
            return NeighborhoodOutcome<ToySolution>::exhausted(1U);
        }});
    neighborhoods.push_back(Neighborhood<ToySolution>{
        "N2-Swap", true,
        [&](const ToySolution& solution, NeighborhoodContext&) {
            calls.push_back("N2");
            clock.advance(1U);
            if (solution.score > 5) {
                return NeighborhoodOutcome<ToySolution>::improved(
                    ToySolution{5}, 2U);
            }
            return NeighborhoodOutcome<ToySolution>::exhausted(2U);
        }});

    VndEngine<ToySolution> engine{
        std::move(neighborhoods), objective, StrictMinimizationAcceptance{0.0}};
    const auto result = engine.run(
        ToySolution{10}, "toy-run", random, clock, stop, observer);

    require_equal(result.solution.score, 5, "best toy score mismatch");
    require_equal(result.termination, SearchTermination::local_optimum,
                  "toy search should reach a registry local optimum");
    require_equal(calls,
                  std::vector<std::string>{"N1", "N1", "N2", "N1", "N2"},
                  "an accepted improvement must restart at N1");
    require_equal(result.progress.accepted_improvements, std::size_t{2},
                  "accepted improvement count mismatch");
    require_equal(result.progress.work_units, std::size_t{7},
                  "work accounting mismatch");
    require_equal(observer.events().size(), std::size_t{6},
                  "trace should contain five results and one completion");
    std::vector<std::size_t> trace_work_units;
    for (const auto& event : observer.events()) {
        trace_work_units.push_back(event.work_units);
    }
    require_equal(trace_work_units,
                  std::vector<std::size_t>{1U, 2U, 4U, 5U, 7U, 7U},
                  "trace work units must be cumulative");
    require_equal(observer.events().front().run_id, std::string{"toy-run"},
                  "trace run id mismatch");
    require(observer.golden_trace().back().find("registry-local-optimum") !=
                std::string::npos,
            "completion trace detail mismatch");
}

void test_fixed_work_stops_before_n2() {
    bool n2_called{};
    ToyClock clock;
    ToyRandom random{{3U}};
    FixedWorkStopPolicy stop{2U};
    NullObserver observer;

    std::vector<Neighborhood<ToySolution>> neighborhoods{
        Neighborhood<ToySolution>{
            "N1-Increment", true,
            [](const ToySolution&, NeighborhoodContext&) {
                return NeighborhoodOutcome<ToySolution>::exhausted(2U);
            }},
        Neighborhood<ToySolution>{
            "N2-Swap", true,
            [&](const ToySolution&, NeighborhoodContext&) {
                n2_called = true;
                return NeighborhoodOutcome<ToySolution>::exhausted();
            }},
    };

    VndEngine<ToySolution> engine{
        std::move(neighborhoods), objective, StrictMinimizationAcceptance{0.0}};
    const auto result = engine.run(
        ToySolution{9}, "toy-work-limit", random, clock, stop, observer);

    require_equal(result.termination, SearchTermination::interrupted,
                  "fixed work should interrupt the registry scan");
    require_equal(result.termination_reason, std::string{"fixed-work-limit"},
                  "fixed-work reason mismatch");
    require(!n2_called, "N2 ran after the fixed-work limit");
}

void test_deadline_can_stop_before_first_call() {
    bool called{};
    ToyClock clock{5U};
    ToyRandom random{{7U}};
    DeadlineStopPolicy stop{5U};
    NullObserver observer;
    std::vector<Neighborhood<ToySolution>> neighborhoods{
        Neighborhood<ToySolution>{
            "N1-Increment", true,
            [&](const ToySolution&, NeighborhoodContext&) {
                called = true;
                return NeighborhoodOutcome<ToySolution>::exhausted();
            }},
    };

    VndEngine<ToySolution> engine{
        std::move(neighborhoods), objective, StrictMinimizationAcceptance{0.0}};
    const auto result = engine.run(
        ToySolution{4}, "toy-deadline", random, clock, stop, observer);

    require_equal(result.termination, SearchTermination::interrupted,
                  "deadline should interrupt before the first call");
    require(!called, "a neighborhood ran after the deadline");
}

void test_interrupted_outcome_stops_without_local_optimum() {
    bool n2_called{};
    ToyClock clock;
    ToyRandom random{{13U}};
    NeverStopPolicy stop;
    VectorTraceObserver observer;
    std::vector<Neighborhood<ToySolution>> neighborhoods{
        Neighborhood<ToySolution>{
            "N1-Increment", true,
            [](const ToySolution&, NeighborhoodContext&) {
                return NeighborhoodOutcome<ToySolution>::interrupted(
                    "toy-scan-stop", 3U, true);
            }},
        Neighborhood<ToySolution>{
            "N2-Swap", true,
            [&](const ToySolution&, NeighborhoodContext&) {
                n2_called = true;
                return NeighborhoodOutcome<ToySolution>::exhausted();
            }},
    };

    VndEngine<ToySolution> engine{
        std::move(neighborhoods), objective, StrictMinimizationAcceptance{0.0}};
    const auto result = engine.run(
        ToySolution{8}, "toy-interrupted", random, clock, stop, observer);

    require_equal(result.termination, SearchTermination::interrupted,
                  "interrupted outcome should stop the search");
    require(!result.registry_local_optimum,
            "interrupted outcome must not claim a local optimum");
    require_equal(result.termination_reason, std::string{"toy-scan-stop"},
                  "interrupted reason mismatch");
    require_equal(result.progress.cap_hits, std::size_t{1},
                  "explicit work limit hit should be counted");
    require(!n2_called, "N2 ran after an interrupted outcome");
    require_equal(observer.events().size(), std::size_t{2},
                  "interruption should emit one result and one stop event");
}

void test_disabled_registry_has_explicit_termination() {
    ToyClock clock;
    ToyRandom random{{5U}};
    NeverStopPolicy stop;
    NullObserver observer;
    std::vector<Neighborhood<ToySolution>> neighborhoods{
        Neighborhood<ToySolution>{"N1-Increment", false, {}},
        Neighborhood<ToySolution>{"N2-Swap", false, {}},
    };

    VndEngine<ToySolution> engine{
        std::move(neighborhoods), objective, StrictMinimizationAcceptance{0.0}};
    const auto result = engine.run(
        ToySolution{6}, "toy-disabled", random, clock, stop, observer);

    require_equal(result.termination,
                  SearchTermination::no_enabled_neighborhoods,
                  "disabled registry termination mismatch");
    require_equal(result.progress.neighborhood_calls, std::size_t{0},
                  "disabled neighborhoods must not be called");
}

void test_acceptance_and_neighborhood_change_are_deterministic() {
    const StrictMinimizationAcceptance acceptance{0.1};
    require(acceptance.accepts(10.0, 9.8),
            "strictly better candidate should be accepted");
    require(!acceptance.accepts(10.0, 9.95),
            "candidate inside the tolerance should be rejected");

    SequentialNeighborhoodChange change{{1, 3}};
    require_equal(change.current_level(), 1,
                  "initial neighborhood level mismatch");
    change.on_rejected();
    require_equal(change.current_level(), 3,
                  "rejection should advance the neighborhood level");
    change.on_rejected();
    require_equal(change.current_level(), 1,
                  "levels should cycle deterministically");
    require_equal(change.completed_cycles(), std::size_t{1},
                  "completed cycle count mismatch");
    change.on_rejected();
    change.on_accepted();
    require_equal(change.current_level(), 1,
                  "acceptance should reset the neighborhood level");
}

void test_optional_value_supports_non_default_constructible_types() {
    const OptionalValue<NonDefaultValue> empty;
    require(!empty, "default optional should be empty");

    const OptionalValue<NonDefaultValue> present{NonDefaultValue{17}};
    const OptionalValue<NonDefaultValue> copied = present;
    require(static_cast<bool>(present),
            "constructed optional should contain a value");
    require_equal(copied.value().value, 17,
                  "copied optional value mismatch");

    bool empty_access_rejected{};
    try {
        const NonDefaultValue& unexpected = empty.value();
        static_cast<void>(unexpected);
    } catch (const std::logic_error&) {
        empty_access_rejected = true;
    }
    require(empty_access_rejected, "empty optional access must throw");
}

void test_invalid_public_contracts_fail_fast() {
    bool missing_callback_rejected{};
    try {
        std::vector<Neighborhood<ToySolution>> neighborhoods{
            Neighborhood<ToySolution>{"N1-Increment", true, {}},
        };
        const VndEngine<ToySolution> engine{
            std::move(neighborhoods), objective,
            StrictMinimizationAcceptance{0.0}};
        static_cast<void>(engine);
    } catch (const std::invalid_argument&) {
        missing_callback_rejected = true;
    }
    require(missing_callback_rejected,
            "enabled neighborhood without a callback must be rejected");

    bool disabled_outcome_rejected{};
    try {
        ToyClock clock;
        ToyRandom random{{19U}};
        NeverStopPolicy stop;
        NullObserver observer;
        std::vector<Neighborhood<ToySolution>> neighborhoods{
            Neighborhood<ToySolution>{
                "N1-Increment", true,
                [](const ToySolution&, NeighborhoodContext&) {
                    return NeighborhoodOutcome<ToySolution>::disabled();
                }},
        };
        const VndEngine<ToySolution> engine{
            std::move(neighborhoods), objective,
            StrictMinimizationAcceptance{0.0}};
        static_cast<void>(engine.run(
            ToySolution{1}, "toy-invalid-outcome", random, clock, stop,
            observer));
    } catch (const std::logic_error&) {
        disabled_outcome_rejected = true;
    }
    require(disabled_outcome_rejected,
            "enabled callback must not return disabled");

    bool non_finite_objective_rejected{};
    try {
        ToyClock clock;
        ToyRandom random{{23U}};
        NeverStopPolicy stop;
        NullObserver observer;
        std::vector<Neighborhood<ToySolution>> neighborhoods{
            Neighborhood<ToySolution>{
                "N1-Increment", true,
                [](const ToySolution&, NeighborhoodContext&) {
                    return NeighborhoodOutcome<ToySolution>::exhausted();
                }},
        };
        const VndEngine<ToySolution> engine{
            std::move(neighborhoods),
            [](const ToySolution&) {
                return std::numeric_limits<double>::infinity();
            },
            StrictMinimizationAcceptance{0.0}};
        static_cast<void>(engine.run(
            ToySolution{1}, "toy-non-finite", random, clock, stop, observer));
    } catch (const std::invalid_argument&) {
        non_finite_objective_rejected = true;
    }
    require(non_finite_objective_rejected,
            "non-finite objective must be rejected");
}

}  // namespace

int main() {
    using Test = std::pair<std::string, std::function<void()>>;
    const std::vector<Test> tests{
        {"improvements_restart_at_n1", test_improvements_restart_at_n1},
        {"fixed_work_stops_before_n2", test_fixed_work_stops_before_n2},
        {"deadline_can_stop_before_first_call",
         test_deadline_can_stop_before_first_call},
        {"interrupted_outcome_stops_without_local_optimum",
         test_interrupted_outcome_stops_without_local_optimum},
        {"disabled_registry_has_explicit_termination",
         test_disabled_registry_has_explicit_termination},
        {"acceptance_and_neighborhood_change_are_deterministic",
         test_acceptance_and_neighborhood_change_are_deterministic},
        {"optional_value_supports_non_default_constructible_types",
         test_optional_value_supports_non_default_constructible_types},
        {"invalid_public_contracts_fail_fast",
         test_invalid_public_contracts_fail_fast},
    };

    std::size_t passed{};
    for (const auto& test : tests) {
        try {
            test.second();
            ++passed;
            std::cout << "[PASS] " << test.first << '\n';
        } catch (const std::exception& error) {
            std::cerr << "[FAIL] " << test.first << ": " << error.what()
                      << '\n';
            return 1;
        }
    }

    std::cout << passed << " toy tests passed\n";
    return 0;
}
