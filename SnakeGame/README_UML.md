# Инструкция по созданию UML диаграммы классов

## Вариант 1: VS Code с расширением PlantUML (рекомендуется)

1. **Установите расширение:**
   - Откройте VS Code
   - Нажмите `Ctrl+Shift+X` (Extensions)
   - Найдите "PlantUML" (автор: jebbs)
   - Установите расширение

2. **Установите Java (если еще не установлен):**
   - PlantUML требует Java для работы
   - Скачайте: https://www.java.com/download/
   - Или установите через Chocolatey: `choco install jre`

3. **Использование:**
   - Откройте файл `.puml` в VS Code
   - Нажмите `Alt+D` для предпросмотра диаграммы
   - Или правой кнопкой → "Preview Current Diagram"
   
4. **Экспорт в изображение (PNG/SVG):**
   - **Способ 1 (через палитру команд):**
     - Нажмите `Ctrl+Shift+P`
     - Введите "PlantUML: Export Current Diagram"
     - Выберите формат (PNG, SVG, EPS, PDF)
     - Файл сохранится рядом с исходным `.puml` файлом
   
   - **Способ 2 (через контекстное меню):**
     - Правой кнопкой на открытом `.puml` файле
     - "Export Current Diagram"
     - Выберите формат
   
   - **Способ 3 (в предпросмотре):**
     - В окне предпросмотра (Alt+D) нажмите правой кнопкой
     - "Export Diagram" → выберите формат

## Вариант 2: Draw.io (онлайн или десктоп)

1. **Откройте Draw.io:**
   - Онлайн: https://app.diagrams.net/
   - Или скачайте десктопную версию: https://github.com/jgraph/drawio-desktop/releases

2. **Создайте диаграмму:**
   - File → New → Blank Diagram
   - Используйте файл `ClassDiagram_Simple.txt` как референс
   - Добавляйте классы через панель инструментов слева

3. **Структура:**
   - GameObject (абстрактный) - базовый класс
   - Ball, Paddle, Block - наследники GameObject
   - SimpleBlock, StrongBlock, BonusBlock, UnbreakableBlock - наследники Block
   - Game - содержит Ball, Paddle, и vector<Block>

## Вариант 3: Онлайн-редакторы PlantUML (альтернативы)

Если основной сайт PlantUML не работает, попробуйте:

1. **Kroki.io (рекомендуется как альтернатива):**
   - Откройте: https://kroki.io/
   - Выберите "PlantUML" в выпадающем списке
   - Вставьте содержимое `.puml` файла
   - Диаграмма отобразится автоматически

2. **Другие онлайн-редакторы:**
   - https://plantuml-editor.kkeisuke.com/
   - https://www.planttext.com/
   - https://www.plantuml.com/plantuml/uml/ (официальный сервер)

Скопируйте содержимое `ClassDiagram.puml` или `ATM_State_Pattern.puml` и вставьте в редактор.

## Вариант 4: Экспорт через командную строку (если встроенный экспорт не работает)

1. **Скачайте PlantUML JAR:**
   - Перейдите: https://plantuml.com/download
   - Скачайте `plantuml.jar`

2. **Экспорт в PNG:**
   ```bash
   java -jar plantuml.jar ATM_State_Pattern.puml
   ```
   Создаст файл `ATM_State_Pattern.png` в той же папке

3. **Экспорт в SVG:**
   ```bash
   java -jar plantuml.jar -tsvg ATM_State_Pattern.puml
   ```

4. **Экспорт в PDF:**
   ```bash
   java -jar plantuml.jar -tpdf ATM_State_Pattern.puml
   ```

## Вариант 5: Экспорт из онлайн редакторов

**Kroki.io:**
- После отображения диаграммы нажмите правой кнопкой на изображение
- "Сохранить изображение как..." (Save image as...)

**PlantText / PlantUML Editor:**
- Обычно есть кнопка "Export" или "Download"
- Выберите формат (PNG, SVG)

## Вариант 6: Visual Studio (встроенные инструменты)

Если у вас Visual Studio Enterprise:
- View → Class View
- Правой кнопкой на проект → View Class Diagram
- Это автоматически создаст диаграмму из кода

## Файлы диаграммы:

- `ClassDiagram.puml` - полная версия диаграммы классов Arkanoid в формате PlantUML
- `ClassDiagram_Simple.txt` - упрощенная текстовая версия для референса
- `ATM_State_Pattern.puml` - диаграмма паттерна State для банкомата

