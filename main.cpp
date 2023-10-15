#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

struct Automaton {
    int states;
    int startState;
    std::vector<int> acceptingStates;
    std::unordered_map<int, std::unordered_map<char, int>> transitions;
};

Automaton readAutomatonFile(const std::string &fileName) {
    Automaton automaton;
    std::ifstream file(fileName);

    if (!file || file.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "Не вдалося відкрити файл" << std::endl;
        exit(1);
    }

    file >> automaton.states;
    file >> automaton.startState;

    int finalStates;
    file >> finalStates;
    automaton.acceptingStates.resize(finalStates);
    for (int i = 0; i < finalStates; ++i) {
        file >> automaton.acceptingStates[i];
    }

    int from, to;
    char transitionSymbol;
    while (file >> from >> transitionSymbol >> to) {
        automaton.transitions[from][transitionSymbol] = to;
    }

    file.close();
    return automaton;
}

std::vector<int> computePrefixFunction(const char* pattern) {
    int m = 0;
    while (pattern[m] != '\0') {
        ++m;
    }

    std::vector<int> prefixFunction(m, 0);
    int k = 0;
    for (int q = 1; q < m; ++q) {
        while (k > 0 && pattern[k] != pattern[q]) {
            k = prefixFunction[k - 1];
        }
        if (pattern[k] == pattern[q]) {
            k++;
        }
        prefixFunction[q] = k;
    }
    return prefixFunction;
}

bool hasPeriodicFragment(const char* word) {
    int n = 0;
    while (word[n] != '\0') {
        ++n;
    }

    std::vector<int> prefixFunction = computePrefixFunction(word);

    // Періодичний фрагмент буде присутній, якщо останній елемент префікс-функції ділить n на (n - (останній елемент))
    int lastPrefix = prefixFunction[n - 1];
    return lastPrefix > 0 && n % (n - lastPrefix) == 0;
}

bool acceptWord(Automaton fa, const char word[]) {
    int currentState = fa.startState;
    for (const char* ptr = word; *ptr != '\0'; ++ptr) {
        char ch = *ptr;

        if (fa.transitions.at(currentState).find(ch) == fa.transitions.at(currentState).end()) {
            return false;
        }

        currentState = fa.transitions.at(currentState).at(ch);
    }

    for (int state : fa.acceptingStates) {
        if (currentState == state)
        {
            return !hasPeriodicFragment(word);
        }
    }
    return false;
}

int main() {
    std::vector<std::string> acceptedWords;

    Automaton automaton = readAutomatonFile("/Users/kcherechecha/CLionProjects/untitled25/finite_automata2.txt");

    std::ifstream inputFile("/Users/kcherechecha/CLionProjects/untitled25/words.txt");

    if (!inputFile.is_open() || inputFile.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "Не вдалося відкрити файл зі словами\n";
        return 1;
    }

    std::vector<std::string> words;

    std::string word;
    while (inputFile >> word) {
        words.push_back(word);
    }

    std::cout << "Слова без періодичних фрагментів, які приймає автомат: " << std::endl;
    for (const auto& w : words) {
        const char* word = w.c_str();
        if (acceptWord(automaton, word)) {
            std::cout << word << std::endl;
        }
    }

    return 0;
}