FROM ubuntu:latest

# Устанавливаем необходимые пакеты
RUN apt-get update && apt-get install -y \
    build-essential \
    strace

# Копируем исходный код программы в контейнер
COPY . /app

# Устанавливаем рабочую директорию
WORKDIR /app

# Компилируем программу (если нужно)

RUN gcc -shared -o mc_cusic.so -fPIC mc_cusic_karels_allocator/allocator.c -lm
RUN gcc -shared -o two_deg.so -fPIC two_degree_allocator/allocator.c -lm
RUN gcc -lm main.c

# Команда по умолчанию
CMD ["/bin/bash"]
