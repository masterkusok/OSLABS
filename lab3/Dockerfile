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
RUN g++ --std=c++17 main.cpp multitasking/child.cpp multitasking/parent.cpp iolib/io.cpp errlib/error_handlers.cpp

# Команда по умолчанию
CMD ["/bin/bash"]
