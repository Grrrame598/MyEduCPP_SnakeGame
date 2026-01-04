# Объяснение: Чисто виртуальные методы (`= 0`)

## Что такое `= 0`?

**`= 0`** - это **синтаксическая конструкция C++**, которая означает: **"чисто виртуальный метод"** (pure virtual function).

Это **НЕ**:
- ❌ Математическое равенство нулю
- ❌ NULL или nullptr
- ❌ Пустой указатель

Это **специальный синтаксис**, который говорит компилятору: 
> "Этот метод **обязательно** должен быть реализован в классах-наследниках, 
> а в базовом классе реализации **не будет**."

---

## Типы методов в базовом классе

### 1. Обычный метод (не виртуальный)
```cpp
sf::Vector2f getPosition() const { return position; }
```
- ✅ Есть реализация
- ✅ Можно вызывать сразу
- ❌ Нельзя переопределить в наследнике (компилятор не предупредит)

### 2. Виртуальный метод (virtual)
```cpp
virtual void move() { position.x += 1.0f; }  // Пример
```
- ✅ Есть реализация по умолчанию
- ✅ Можно вызывать (будет использована реализация из базового класса)
- ✅ Можно переопределить в наследнике

### 3. Чисто виртуальный метод (virtual ... = 0)
```cpp
virtual void update(float deltaTime, const sf::RenderWindow* window = nullptr) = 0;
```
- ❌ НЕТ реализации в базовом классе
- ❌ НЕЛЬЗЯ вызвать (компилятор не даст)
- ✅ **ОБЯЗАТЕЛЬНО** должен быть реализован в наследниках

---

## Что происходит, если не реализовать чисто виртуальный метод?

### ❌ Ошибка компиляции:

```cpp
class Ball : public GameObject
{
public:
    Ball();
    // Забыли реализовать update() - ОШИБКА!
    // Забыли реализовать draw() - ОШИБКА!
    // Забыли реализовать getBounds() - ОШИБКА!
};
```

**Компилятор скажет:**
```
error: cannot declare variable 'ball' to be of abstract type 'Ball'
note:   because the following virtual functions are pure within 'Ball':
note:     virtual void GameObject::update(float, const sf::RenderWindow*) = 0
note:     virtual void GameObject::draw(sf::RenderWindow&) = 0
note:     virtual sf::FloatRect GameObject::getBounds() const = 0
```

---

## Можно ли создать объект класса с чисто виртуальными методами?

### ❌ НЕТ, нельзя:

```cpp
GameObject obj;  // ❌ ОШИБКА компиляции!
```

**Компилятор скажет:**
```
error: cannot declare variable 'obj' to be of abstract type 'GameObject'
note:   because the following virtual functions are pure within 'GameObject':
note:     virtual void GameObject::update(float, const sf::RenderWindow*) = 0
note:     virtual void GameObject::draw(sf::RenderWindow&) = 0
note:     virtual sf::FloatRect GameObject::getBounds() const = 0
```

### ✅ Можно через указатель/ссылку на базовый класс:

```cpp
Ball ball;              // ✅ OK
Paddle paddle;          // ✅ OK

GameObject* obj1 = &ball;     // ✅ OK - указатель на базовый класс
GameObject* obj2 = &paddle;   // ✅ OK - указатель на базовый класс

obj1->update(0.1f);     // ✅ Вызовется Ball::update()
obj2->update(0.1f, &window);  // ✅ Вызовется Paddle::update()
```

---

## Зачем это нужно?

### 1. **Гарантия реализации**
Компилятор **заставляет** реализовать методы, без этого код не скомпилируется.

### 2. **Интерфейс (контракт)**
Базовый класс говорит: 
> "Все объекты должны уметь обновляться (`update()`), 
> рисоваться (`draw()`), и предоставлять свои границы (`getBounds()`), 
> но я не знаю, КАК это делать - каждый реализует сам."

### 3. **Полиморфизм**
Можно работать с разными объектами через указатель на базовый класс:

```cpp
std::vector<GameObject*> objects;
objects.push_back(&ball);
objects.push_back(&paddle);

for (auto* obj : objects) {
    obj->update(deltaTime, &window);  // Вызывается правильный метод для каждого объекта
    obj->draw(window);                // Вызывается правильный метод для каждого объекта
}
```

---

## Почему `= 0`?

Это просто **исторический синтаксис C++**. В других языках используются другие слова:
- Java, C#: `abstract` метод
- Python: нет специального синтаксиса (можно использовать `pass` или `raise NotImplementedError`)

В C++ решили использовать `= 0`, потому что это похоже на "обнуление" функции - 
убрать реализацию, оставить только объявление.

---

## Сравнение с нашим кодом

### В GameObject (базовый класс):
```cpp
virtual void update(...) = 0;  // ← Чисто виртуальный, нет реализации
virtual void draw(...) = 0;    // ← Чисто виртуальный, нет реализации
sf::Vector2f getPosition() const { return position; }  // ← Обычный, есть реализация
```

### В Ball (наследник):
```cpp
void update(...) override { ... }  // ← Обязательно реализуем
void draw(...) override { ... }    // ← Обязательно реализуем
// getPosition() - не пишем, используем из базового класса ✅
```

### В Paddle (наследник):
```cpp
void update(...) override { ... }  // ← Обязательно реализуем (своя логика!)
void draw(...) override { ... }    // ← Обязательно реализуем (своя логика!)
// getPosition() - не пишем, используем из базового класса ✅
```

---

## Итог

**`= 0`** означает: 
- "Этот метод **обязательно** должен быть реализован в наследниках"
- "В базовом классе реализации **нет**"
- "Нельзя создать объект базового класса"

Это способ создать **интерфейс** или **абстрактный класс** в C++.

