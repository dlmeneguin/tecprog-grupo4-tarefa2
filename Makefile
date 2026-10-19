# Nome da biblioteca (gera libqsim.a e libqsim.so)
LIBS   = qsim
# Flags implementadas: -fPIC (gera .so), -MMD -MP (gera .d com dependências de headers), -Iinclude (acha os .h)
CFLAGS = -fPIC -MMD -MP -Iinclude
# Bibliotecas na hora de linkar: -lm (matemática)
LDLIBS = -lm
# Destino do make install
PREFIX ?= /usr/local

# Listas dos arquivos existentes: SRCS (código-fonte dos módulos), APP_SRCS (código-fonte do app),
# TESTS_SRCS (código-fonte dos testes), OBJS e APP_OBJS (código de máquina .o dos módulos e do app),
# TESTS (executáveis dos testes)
SRCS       = $(wildcard src/*.c)
APP_SRCS   = $(wildcard app/*.c)
TESTS_SRCS = $(wildcard tests/*.c)
OBJS       = $(SRCS:%.c=build/%.o)
APP_OBJS   = $(APP_SRCS:%.c=build/%.o)
TESTS      = $(TESTS_SRCS:%.c=build/%)

# Nomes das bibliotecas estática e dinâmica
STATIC    = lib/lib$(LIBS).a
SHARED    = lib/lib$(LIBS).so

# Cria as pastas de saída build, lib e bin, se ainda não existirem
$(shell mkdir -p build/src build/app build/tests lib bin)

# Alvo padrão: bibliotecas e executáveis
all: $(STATIC) $(SHARED) bin/app_static bin/app_shared

# Compila qualquer .c em .o ($< = fonte, $@ = objeto)
build/%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

# Biblioteca estática: empacota os .o
$(STATIC): $(OBJS)
	ar rcs $(STATIC) $(OBJS)

# Biblioteca dinâmica
$(SHARED): $(OBJS)
	gcc -shared -o $(SHARED) $(OBJS) $(LDLIBS)

# Executável com a biblioteca copiada para dentro dele
bin/app_static: $(APP_OBJS) $(STATIC)
	gcc $(APP_OBJS) $(STATIC) -o bin/app_static $(LDLIBS)

# rpath: o executável procura a .so em ../lib (relativo a ele mesmo)
bin/app_shared: $(APP_OBJS) $(SHARED)
	gcc $(APP_OBJS) -Llib -l$(LIBS) -Wl,-rpath,'$$ORIGIN/../lib' -o bin/app_shared $(LDLIBS)

# Roda o app com a biblioteca estática e dinâmica
run: all
	./bin/app_static
	./bin/app_shared

# Pra cada tests/x.c cria um executável build/tests/x
build/tests/%: tests/%.c $(STATIC)
	gcc $(CFLAGS) $< $(STATIC) -o $@ $(LDLIBS)

# Roda todos os testes e para no primeiro que falhar
test: $(TESTS)
	@for t in $(TESTS); do ./$$t && echo "PASS $$t" || { echo "FAIL $$t"; exit 1; }; done

# Instala a biblioteca e os arquivos da interface
install: all
	mkdir -p $(PREFIX)/lib $(PREFIX)/include/$(LIBS)
	cp $(STATIC) $(SHARED) $(PREFIX)/lib
	cp include/*.h $(PREFIX)/include/$(LIBS)

# Remove os arquivos e pastas gerados pela compilação
clean:
	rm -rf build/* lib/*.a lib/*.so bin/*

# Recompila quando um header muda (ignora se os .d ainda não existem)
-include $(wildcard build/*/*.d)
