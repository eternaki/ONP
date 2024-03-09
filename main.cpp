#include <iostream>
#include "string.h"

enum ItemType {
    MULTIPLICATION,
    DIVISION,
    ADDITION,
    SUBSTRACTION,
    APPLICATION,
    NEGATION,
    CONDITION,
    LEFT_PAREN,
    RIGHT_PAREN,
    ARG_SEPARATION,
    NUMBER,
    STOP,
    MAXIMUM,
    MINIMUM
};

class Item {
    ItemType type;
    int value;
public:
    Item() {};
    Item(ItemType type) { this->type = type; };
    Item(int value) {
        this->type = NUMBER;
        this->value = value;
    }
    operator int() { // check Int type
        if (type != NUMBER) throw "item is not number";
        return value;
    }
    ItemType getType() { return type; } // return type of data
    bool isOperand() {
        return this->type != NUMBER;
    }
    int priority() { // For ) ( + - IF MAX MIN * /
        switch (type) {
            case SUBSTRACTION:
            case ADDITION: return 1;
            case MULTIPLICATION:
            case DIVISION: return 2;
            case MINIMUM:
            case MAXIMUM:
            case NEGATION:
            case CONDITION: return 3;
            case RIGHT_PAREN:
            case LEFT_PAREN: return 0;
            case NUMBER: printf("cant call priority on number"); exit(1);
            default: exit(1);
        }
    }
    friend std::ostream &operator<<(std::ostream &out, Item item);
};

std::ostream &operator<<(std::ostream &out, Item item) {
    const char *str;
    switch (item.type) {
        case SUBSTRACTION: str = "-"; break;
        case ADDITION: str = "+"; break;
        case DIVISION: str = "/"; break;
        case MULTIPLICATION: str = "*"; break;
        case NEGATION: str = "N"; break;
        case MAXIMUM: str = "MAX"; break;
        case MINIMUM: str = "MIN"; break;
        case CONDITION: str = "IF"; break;
        case LEFT_PAREN: str = "("; break;
        case RIGHT_PAREN: str = ")"; break;
        case NUMBER: out << item.value; return out;
    }
    return out << str;
}

struct StackNode {
    Item item;
    StackNode *last = nullptr;
    StackNode(Item &item) { this->item = item; }
};


class Stack {
private:
    StackNode *last = nullptr;
public:
    void push(Item item) {
        StackNode *node = new StackNode(item);
        node->last = this->last;
        this->last = node;
    }
    bool pop(Item &item) {
        if (last == nullptr) return false;
        item = this->last->item;
        StackNode *old_node = this->last;
        this->last = this->last->last;
        delete old_node;
        return true;
    }
    Stack &show() {
        for (StackNode *current = last;
             current != nullptr;
             current = current->last) {
            std::cout << current->item << " ";
        }
        std::cout << "\n";
        return *this;
    }
    Stack &reverse() {
        Stack reversed;
        Item item;
        while (pop(item)) {
            reversed.push(item);
        }
        *this = reversed;
        reversed.last = nullptr;
        return *this;
    }
    ~Stack() {
        while (last != nullptr) {
            StackNode *temp = last;
            last = last->last;
            delete temp;
        }
    }

    Item top() {
        return last->item;
    }

    void add() {
        Item value1, value2;
        pop(value1);
        pop(value2);
        push((int)value1 + (int)value2);
    }

    void sub() {
        Item value1, value2;
        pop(value1);
        pop(value2);
        push((int)value2 - (int)value1);
    }

    void mul() {
        Item value1;
        Item value2;
        pop(value1);
        pop(value2);
        push((int)value1 * (int)value2);
    }

    int div() {
        Item value1;
        Item value2;
        pop(value1);
        pop(value2);
        if (value1 == 0) {
            return 1;
        }
        push((int)value2 / (int)value1);
        return 0;
    }

    void neg() {
        Item value;
        pop(value);
        push(-(int)value);
    }

    void cond() {
        Item first, second, third;
        pop(first);
        pop(second);
        pop(third);
        if (third > 0) {
            push(second);
        } else {
            push(first);
        }
    }

    void max() {
        Item maxVal = top();
        Item current;
        while (pop(current)) {
            if (current > maxVal) {
                maxVal = current;
            }
        }
        push(maxVal);
    }

    void min() {
        Item minVal = top();
        Item current;
        while (pop(current)) {
            if (current < minVal) {
                minVal = current;
            }
        }
        push(minVal);
    }

    int evaluateExpression(int &result) {
        Item operand;
        Stack stack;
        int error = 0;
        while (pop(operand) && !error) {
            if (operand.isOperand()) {
                std::cout << operand << " ";
                stack.show();
            }
            switch (operand.getType()) {
                case SUBSTRACTION: stack.sub(); break;
                case ADDITION: stack.add(); break;
                case MULTIPLICATION: stack.mul(); break;
                case DIVISION: error = stack.div(); break;
                case NEGATION: stack.neg(); break;
                case MAXIMUM: stack.max(); break;
                case MINIMUM: stack.min(); break;
                case CONDITION: stack.cond(); break;
                case NUMBER: stack.push(operand); break;
            }
        }
        if (!error) result = stack.top();
        return error;
    }
};


class Reader {
public:
    Item readItem() {
        int value;
        char str[124];
        std::cin >> str;
        if (strcmp(str, "+") == 0) return Item(ADDITION);
        else if (strcmp(str, "-") == 0) return Item(SUBSTRACTION);
        else if (strcmp(str, "*") == 0) return Item(MULTIPLICATION);
        else if (strcmp(str, "/") == 0) return Item(DIVISION);
        else if (strcmp(str, "N") == 0) return Item(NEGATION);
        else if (strcmp(str, "IF") == 0) return Item(CONDITION);
        else if (strcmp(str, "MAX") == 0) return Item(MAXIMUM);
        else if (strcmp(str, "MIN") == 0) return Item(MINIMUM);
        else if (strcmp(str, "(") == 0) return Item(LEFT_PAREN);
        else if (strcmp(str, ")") == 0) return Item(RIGHT_PAREN);
        else if (strcmp(str, ",") == 0) return Item(ARG_SEPARATION);
        else if (strcmp(str, ".") == 0) return Item(STOP);
        else {
            return Item(atoi(str));
        }
    }

    void readExpression(Stack *exp) {
        Stack operands;
        Item item;
        for (;;) {
            item = readItem();
            if (item.getType() == STOP)
                break;
            else if (item.getType() == ARG_SEPARATION) {
                // force the evaluation of all the operands
                // before ,
                while (operands.pop(item)) {
                    if (item.getType() == LEFT_PAREN) {
                        break;
                    }
                    exp->push(item);
                }
                operands.push(Item(LEFT_PAREN));
            } else if (item.getType() == LEFT_PAREN)
                operands.push(item);
            else if (item.getType() == RIGHT_PAREN) {
                // take out all the items that have higher
                // precedence
                // this is because they should be executed
                // before the lower precedence operators
                while (operands.pop(item)) {
                    if (item.getType() == LEFT_PAREN) {
                        break;
                    }
                    exp->push(item);
                }
            } else if (item.getType() == NEGATION ||
                       item.getType() == CONDITION ||
                       item.getType() == MAXIMUM ||
                       item.getType() == MINIMUM) {
                operands.push(item);
            } else if (item.isOperand()) {
                Item oper;
                while (operands.pop(oper)) {
                    if (item.priority() <= oper.priority()) {
                        exp->push(oper);
                    } else {
                        operands.push(oper);
                        break;
                    }
                }
                operands.push(item);
            } else {
                exp->push(item);
            }

        }
        Item exchange;
        while (operands.pop(exchange)) {
            exp->push(exchange);
        }
    }
};

int main() {
    Reader reader;
    int n;
    std::cin >> n;
    int value;
    while (n-- > 0) {
        std::cout << "\n";
        Stack exp;
        reader.readExpression(&exp);
        int error = exp.reverse().show().evaluateExpression(value);
        if (error) {
            std::cout << "ERROR\n";
        } else {
            std::cout << value << "\n";
        }
    }
}
