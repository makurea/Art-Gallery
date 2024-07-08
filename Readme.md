﻿# Картинная галерея

## Описание проекта

### Цель
Создание простой однотабличной базы данных для хранения информации о картинах.

### Функциональность
- **Запись и загрузка файла базы данных (бинарный файл):**
  - Сохранение данных в файл и загрузка из него.
- **Добавление новых записей, удаление и редактирование старых:**
  - Добавление информации о новых картинах.
  - Удаление записей о картинах.
  - Редактирование информации о существующих картинах.
- **Сортировка записей по любому из полей базы данных в любом направлении:**
  - Сортировка записей по названию картины, фамилии художника, дате написания, виду, цене, номеру зала.
  - Возможность сортировки в прямом и обратном порядке.
- **Фильтрация записей по значению любого поля:**
  - Поиск записей по заданным критериям (название картины, фамилия художника, дата написания, вид, цена, номер зала).
- **Поиск записей по значению любого поля:**
  - Поиск записей, содержащих заданную строку (название картины, фамилия художника и т.д.).
- **Дополнительная обработка (с сохранением результата в текстовый файл):**
  - Создание отчета по каждому залу, указывающего общую ценность выставляемых там картин.
  - Сохранение отчетов в текстовые файлы.

## Требования к системе
- **Операционная система:** Windows.
- **Язык программирования:** C++.
- **Библиотеки:** `stdio.h`, `<iostream>` (необязательно).

## Инструкция по запуску

1. Скачайте проект из GitHub.
2. Скомпилируйте проект с помощью компилятора C++.
3. Запустите исполняемый файл.

## Интерфейс

- Программа имеет оконный интерфейс с использованием элементов Win32 API.
- Главное окно содержит поля для ввода информации о картине (название, фамилия художника, дата написания, вид, цена, номер зала).
- Кнопки "Добавить", "Удалить", "Редактировать" позволяют выполнять соответствующие операции с записями в базе данных.
- Выпадающие списки "Сортировать по" и "Направление" позволяют сортировать записи по выбранному полю в выбранном направлении.
- Поле "Фильтр" позволяет фильтровать записи по заданному критерию.
- Кнопка "Поиск" позволяет найти записи, содержащие заданную строку.
- Кнопка "Отчет" позволяет создать отчет по каждому залу, указывающего общую ценность выставляемых там картин.
