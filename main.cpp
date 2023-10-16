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

std::vector<int> computePrefixFunction(const std::string& pattern) {
    int m = pattern.size();

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

bool hasPeriodicFragment(const std::string& word) {
    int n = word.size();

    std::vector<int> prefixFunction = computePrefixFunction(word);

    // Періодичний фрагмент буде присутній, якщо останній елемент префікс-функції ділить n на (n - (останній елемент))
    int lastPrefix = prefixFunction[n - 1];
    return lastPrefix > 0 && n % (n - lastPrefix) == 0;
}


bool acceptWord(Automaton fa, std::string word) {
    int currentState = fa.startState;
    for (const char& ch : word) {
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

std::vector<std::string> generateWords(const Automaton &automaton) {
    std::vector<std::string> acceptedWords;

    for (int len = 1; len <= 7; ++len) {
        for (int i = 0; i < (1 << len); ++i) {
            std::string word;
            for (int j = 0; j < len; ++j) {
                if (i & (1 << j)) word += 'b';
                else word += 'a';
            }
            if (acceptWord(automaton, word)) {
                acceptedWords.push_back(word);
            }
        }
    }

    return acceptedWords;
}



int main() {
    std::vector<std::string> acceptedWords;

    Automaton automaton = readAutomatonFile("../automata3.txt");



    std::vector<std::string> words = generateWords(automaton);

    std::cout << "Слова без періодичних фрагментів, які приймає автомат: " << std::endl;
    for (const auto& w : words) {
        std::cout << "Слово " + w + " приймається автоматом\n";
    }

    return 0;
}