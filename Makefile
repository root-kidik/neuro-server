PRESETS ?= debug release debug-asan-ubsan release-asan-ubsan debug-tsan release-tsan

.PHONY: $(addprefix cmake-, $(PRESETS))
$(addprefix cmake-, $(PRESETS)): cmake-%:
	cmake --preset $*

.PHONY: $(addprefix build-, $(PRESETS))
$(addprefix build-, $(PRESETS)): build-%: cmake-%
	cmake --build --preset $*

.PHONY: $(addprefix unit-test-, $(PRESETS))
$(addprefix unit-test-, $(PRESETS)): unit-test-%: build-%
	ctest --preset $* --output-junit junit.xml -T test --test-dir build-$*

.PHONY: $(addprefix integration-test-, $(PRESETS))
$(addprefix integration-test-, $(PRESETS)): integration-test-%: build-%
	TESTSUITE_ALLOW_ROOT=ON BUILD_TYPE=$* python3 -m pytest -vs test/integration

.PHONY: unit-test-all
unit-test-all: $(addprefix unit-test-, $(PRESETS))

.PHONY: integration-test-all
integration-test-all: $(addprefix integration-test-, $(PRESETS))

.PHONY: test-all
test-all: unit-test-all integration-test-all

.PHONY: clear
clear:
	rm -rf build* *.log *.lock *.out

.PHONY: format
format:
	find src -name '*pp' -type f | xargs clang-format-19 -i

.PHONY: style
style:
	find src -name '*pp' -type f | xargs clang-format -i --dry-run -Werror
