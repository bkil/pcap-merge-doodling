CLANGTIDY := $(shell\
 clang-tidy --version >/dev/null &&\
 echo clang-tidy ||\
 echo true)
VALGRIND := valgrind
VALGRINDEXTRA := $(shell\
 $(VALGRIND) --version |\
 egrep -q "valgrind-(3\.(1[1-9]|[2-9][0-9]+)|[4-9])" &&\
 echo "--expensive-definedness-checks=yes")


CC := gcc
CWARN := \
 -Wno-unused-parameter\
 -Wpedantic\
 -Wwrite-strings\
 -Woverflow\
 -fstrict-overflow\
 -Wstrict-overflow=5 -Wno-error=strict-overflow\
 -Wformat-nonliteral\
 -fstrict-aliasing\
 -Winit-self \
 -fno-common \
 -W \
 -Wabi \
 -Wall \
 -Wcast-align \
 -Wdeprecated \
 -Wdeprecated-declarations \
 -Wextra \
 -Winvalid-pch \
 -Wmissing-format-attribute \
 -Woverflow \
 -Wshadow \
 \
 -Waggregate-return \
 -Wbad-function-cast \
 -Wcast-qual \
 -Wconversion \
 -Wdouble-promotion \
 -Wfloat-equal \
 -Wformat=2 \
 -Winline \
 -Wmissing-declarations \
 -Wno-error=missing-format-attribute \
 -Wmissing-prototypes \
 -Wnested-externs \
 -Wpointer-arith \
 -Wpointer-sign \
 -Wredundant-decls \
 -Wsign-conversion \
 -Wstrict-prototypes \
 -Wsuggest-attribute=pure \
 -Wsuggest-attribute=const \
 -Wsuggest-attribute=noreturn \
 -Wsuggest-attribute=format \
 -Wswitch-default \
 -Wundef
CFLAGS := -Werror -g -std=gnu11 $(CWARN)

COMP := $(CC) $(CFLAGS)
MAININCLUDE := -Ilib -Ilibmain
INCLUDE := -Ilibtest $(MAININCLUDE)
CLANGTIDYOPTS := -- -c -g -O0 -std=gnu11 $(INCLUDE)
SCANBUILD := \
 @scan-build \
 -o var/reports --status-bugs -no-failure-reports \
 -enable-checker alpha.core.BoolAssignment \
 -enable-checker alpha.core.CallAndMessageUnInitRefArg \
 -enable-checker alpha.core.CastSize \
 -enable-checker alpha.core.CastToStruct \
 -enable-checker alpha.core.DynamicTypeChecker \
 -enable-checker alpha.core.FixedAddr \
 -enable-checker alpha.core.IdenticalExpr \
 -enable-checker alpha.core.PointerArithm \
 -enable-checker alpha.core.PointerSub \
 -enable-checker alpha.core.SizeofPtr \
 -enable-checker alpha.core.TestAfterDivZero \
 -enable-checker alpha.deadcode.UnreachableCode \
 -enable-checker alpha.security.ArrayBound \
 -enable-checker alpha.security.ArrayBoundV2 \
 -enable-checker alpha.security.MallocOverflow \
 -enable-checker alpha.security.ReturnPtrRange \
 -enable-checker alpha.security.taint.TaintPropagation \
 -enable-checker alpha.unix.Chroot \
 -enable-checker alpha.unix.PthreadLock \
 -enable-checker alpha.unix.SimpleStream \
 -enable-checker alpha.unix.Stream \
 -enable-checker alpha.unix.cstring.BufferOverlap \
 -enable-checker alpha.unix.cstring.NotNullTerminated \
 -enable-checker alpha.unix.cstring.OutOfBounds \
 -enable-checker llvm.Conventions \
 -enable-checker nullability.NullableDereferenced \
 -enable-checker nullability.NullablePassedToNonnull \
 -enable-checker nullability.NullablePassedToNonnull \
 -enable-checker optin.performance.Padding \
 -enable-checker security.FloatLoopCounter \
 -enable-checker security.insecureAPI.rand \
 -enable-checker security.insecureAPI.strcpy \
 clang -c -g -O0 -std=gnu11 -o /dev/null $(INCLUDE)

# scan-build --help |
# sed -rn "/^AVAILABLE CHECKERS:/,$ s~^   ([^ ]+)( .*)?$~ -enable-checker \1 \\\\~; T e; p; :e"

INDENT := indent --line-length 80
GDB := gdb --return-child-result \
 --batch --eval-command "run" --eval-command "bt" \
 --args

all: $(addsuffix .elf, $(basename $(wildcard main/*.c)))
check: $(addsuffix .elf, $(basename $(wildcard test/*.c)))
	@echo "All tests passed successfully."

format:
	$(INDENT) $(wildcard */*.c */*.h)

test/%.elf: test/%.c libmain/%.o $(addsuffix .o, $(basename $(wildcard lib/*.c libtest/*.c)))
	$(CLANGTIDY) $< $(CLANGTIDYOPTS)
	$(SCANBUILD) $<
	@rmdir var/reports 2>/dev/null; true
	@echo $(CC) -o $@ $<
	@$(COMP) -o $@ \
	 libmain/$(notdir $(addsuffix .o, $(basename $<))) \
	 $(addsuffix .o, $(basename $(wildcard lib/*.c libtest/*.c))) \
	 $(INCLUDE) $<
	$(GDB) $@ || { mv $@ $@~ 2>/dev/null; false; }
	@valgrind \
	 --leak-check=full \
	 --trace-children=yes \
	 --read-inline-info=yes \
	 --read-var-info=yes \
	 --merge-recursive-frames=16 \
	 --leak-check=full \
	 --num-callers=40 \
	 --show-leak-kinds=all \
	 --errors-for-leak-kinds=all \
	 --track-origins=yes \
	 --malloc-fill=42 \
	 --free-fill=84 \
	 --error-exitcode=2 \
	 $(VALGRINDEXTRA) \
	 $@ || { mv $@ $@~ 2>/dev/null; false; }

main/%.elf: main/%.c libmain/%.o $(addsuffix .o, $(basename $(wildcard lib/*.c)))
	$(CLANGTIDY) $< $(CLANGTIDYOPTS)
	$(SCANBUILD) $<
	@rmdir var/reports 2>/dev/null; true
	@echo $(CC) -o $@ $<
	@$(COMP) -o $@ \
	 libmain/$(notdir $(addsuffix .o, $(basename $<))) \
	 $(addsuffix .o, $(basename $(wildcard lib/*.c))) \
	 $(MAININCLUDE) $<

%.o: %.c
	$(CLANGTIDY) $< $(CLANGTIDYOPTS)
	$(SCANBUILD) $<
	@rmdir var/reports 2>/dev/null; true
	@echo $(CC) -o $@ $<
	@$(COMP) -c -o $@ -Ilib $<

clean:
	rm \
	 */*.elf\
	 */*.o\
	 */*~\
	 *~\
	 2>/dev/null; true
	rm -R var/reports 2>/dev/null; true
