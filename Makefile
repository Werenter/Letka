include config.mk
include tasks.mk

.PHONY : all

all: $(TASKS)

clean: 
	rm -rf $(TASKS)

add_task:
	echo "$(TASK_NAME)\\" >> tasks.mk
	cp template.c $(TASK_NAME).c
	echo "$(TASK_NAME)" >> .gitignore
