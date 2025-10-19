# ARC Cache Project

Проект содержит реализации кэшей и сопутствующие тесты.

## Сборка

Проект использует CMake. Поддерживаются отдельные сборки в режимах **Debug** и **Release**.  
В Debug-сборке по умолчанию включён **AddressSanitizer**.

### Подготовка

Создайте директорию для сборки и настройте проект:

```bash
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake -B build-release -DCMAKE_BUILD_TYPE=Release
```

### Сборка

Собрать всё:
```bash
cmake --build build-debug cmake --build build-release
```

Собрать только один исполняемый файл (например, arc_cache_unit_tests):
```bash
cmake --build build-debug --target arc_cache_unit_tests
```

### Отключение санитайзера в Debug

Если нужно собрать Debug без санитайзера:
```bash
cmake -B build-debug-no-san -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=OFF cmake --build build-debug-no-san
```

### Тестирование

Тесты регистрируются через CTest. После сборки в директории сборки выполните:

```bash
cd build-debug ctest
```

Также можно запускать тесты напрямую:
```bash
./arc_cache_unit_tests
./perfect_cache_unit_tests
```

Результат теста tests/input_data/015.dat:

```bash
./build-debug/perfect_cache < tests/input_data/015.dat
Time consumed: 2214721 us
124450
```
