#!/bin/bash

rm -rf ./build

mkdir build && cd build || { echo "Не удалось создать или перейти в директорию build"; exit 1; }

cmake .. || { echo "Ошибка при выполнении cmake .."; exit 1; }

cmake --build . || { echo "Ошибка при сборке проекта"; exit 1; }

echo "Сборка завершена успешно!")
rm -rf ./build

mkdir build && cd build || { echo "Не удалось создать или перейти в директорию build"; exit 1; }

cmake .. || { echo "Ошибка при выполнении cmake .."; exit 1; }

