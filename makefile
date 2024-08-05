# Определяем переменные
SUBDIRS = ./p1 ./p2

# Основная цель: собрать все бинарные файлы во всех подпроектах
all: subdirs

# Правило для сборки всех подпроектов
subdirs:
	@for dir in $(SUBDIRS); do \
		echo "Building in $$dir"; \
		$(MAKE) -C $$dir; \
	done


