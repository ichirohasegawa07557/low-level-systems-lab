.PHONY: all validate test cpu matrix os-source-check os-image run-os clean package
all: validate
cpu:
	$(MAKE) -C cpu_asm_emulator test
matrix:
	$(MAKE) -C asm_matrix_kernel test
os-source-check:
	$(MAKE) -C tiny_os check
validate: cpu matrix os-source-check
	@echo "all portable tests passed"
	@echo "full portable validation passed"
test: validate
os-image:
	$(MAKE) -C tiny_os image
run-os:
	$(MAKE) -C tiny_os run
clean:
	$(MAKE) -C cpu_asm_emulator clean
	$(MAKE) -C asm_matrix_kernel clean
	$(MAKE) -C tiny_os clean
	rm -f low-level-systems-lab-portable-working.zip
package: clean
	cd .. && zip -r low-level-systems-lab-macos-working.zip low-level-systems-lab-macos-working -x '*/.git/*' '*/bin/*' '*/build/*'
