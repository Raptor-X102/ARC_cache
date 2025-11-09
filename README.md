# ARC Cache Project

Проект содержит реализации кэшей и сопутствующие тесты.

## Сборка

Проект использует CMake. Поддерживаются отдельные сборки в режимах **Debug** и **Release**.  
В Debug-сборке по умолчанию включён **AddressSanitizer**.

### Подготовка

Создайте директорию для сборки и настройте проект:

```bash
# Базовая сборка
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake -B build-release -DCMAKE_BUILD_TYPE=Release

# Сборка с замером времени
cmake -B build-debug-time -DCMAKE_BUILD_TYPE=Debug -DENABLE_TIME_MEASURE=ON
cmake -B build-release-time -DCMAKE_BUILD_TYPE=Release -DENABLE_TIME_MEASURE=ON

# Сборка с санитайзерами и замером времени
cmake -B build-debug-full -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON -DENABLE_TIME_MEASURE=ON
```

### Сборка

Debug:
```bash
cmake --build build-debug
```

Release:
```bash
cmake --build build-release
```

С замером времени:
```bash
cmake --build build-debug-time
cmake --build build-release-time
```

Собрать только один исполняемый файл (например, arc_cache_unit_tests):
```bash
cmake --build build-debug --target arc_cache_unit_tests
```

### Замер времени выполнения

Для включения замера времени используйте флаг `-DENABLE_TIME_MEASURE=ON`:

```bash
# Сборка с замером времени
cmake -B build-time -DENABLE_TIME_MEASURE=ON ..
cmake --build build-time

# Запуск с выводом времени
./build-time/arc_cache < input.txt
./build-time/perfect_cache < tests/input_data/015.dat
```

### Отключение санитайзера в Debug

Если нужно собрать Debug без санитайзера:
```bash
cmake -B build-debug-no-san -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=OFF
cmake --build build-debug-no-san
```

### Тестирование

Тесты регистрируются через CTest. После сборки в директории сборки выполните:

```bash
cd tests/build-debug
ctest
```

Также можно запускать тесты напрямую:
```bash
./arc_cache_unit_tests
./perfect_cache_unit_tests
```

### Пример работы с входными данными

Результат теста tests/input_data/015.dat:

```bash
# Сборка с замером времени
./build-debug/perfect_cache < tests/input_data/015.dat
Time consumed: 987189 us
124450
```

## Структура проекта

```
project/
├── headers/                 # Заголовочные файлы
│   ├── ARC_cache.hpp
│   ├── Perfect_cache.hpp
│   ├── Timer.hpp
│   └── Debug_printf.h
├── src/                    # Исходный код
│   ├── ARC_cache_main.cpp
│   ├── Perfect_cache_main.cpp
│   └── test_main.cpp
├── tests/                  # Тесты
│   ├── CMakeLists.txt
│   ├── ARC_cache_tests.cpp
│   ├── Perfect_cache_tests.cpp
│   └── input_data/         # Тестовые данные
└── CMakeLists.txt
```

## Характеристики тестирующей машины

| Устройство                | Huawei Matebook D 16                      |
|---------------------------|------------------------------------------|
| OS                        | Manjaro Linux, KDE Plasma                |
| CPU                       | 13th Gen Intel(R) Core(TM) i9-13900H     |
| Объем оперативной памяти  | 16,0 ГБ                                 |
| Тип системы               | 64-разрядная операционная система, процессор x64 |
