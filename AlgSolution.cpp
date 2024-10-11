#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

const int kLangSize = 26;
const int kRootVal = 0;

class Trie {
 private:
  struct TrieNode {
    std::vector<int> next;
    int maxFrequentIndex;
    int maxFrequency;
    TrieNode() : next(kLangSize, -1), maxFrequentIndex(-1), maxFrequency(0) {}
  };
  
  int root_ = 0;
  int nodeCnt_;
  std::vector<TrieNode> nodePool_;
  std::vector<std::string> wordList_;
  std::unordered_map<std::string, int> wordToIndex_;
  std::unordered_map<int, int> wordCount_;

  void CreateNode() {
    ++nodeCnt_;
    TrieNode new_node;
    nodePool_.push_back(new_node);
  }

  void UpdateFrequency(int nodeNo, int index) {
    if (nodePool_[nodeNo].maxFrequency < wordCount_[index]) {
      nodePool_[nodeNo].maxFrequency = wordCount_[index];
      nodePool_[nodeNo].maxFrequentIndex = index;
    }
  }

 public:
  Trie() : nodeCnt_(0), nodePool_(1) {}

  void InsertWord(const std::string& word) {
    int cur = root_;
    if (wordToIndex_.count(word) == 0) {
      wordToIndex_[word] = wordList_.size();
      wordList_.push_back(word);
    }
    
    int wordIndex = wordToIndex_[word];
    ++wordCount_[wordIndex];

    for (size_t pos = 0; pos < word.size(); ++pos) {
      int curSymb = word[pos] - 'a';
      if (nodePool_[cur].next[curSymb] == -1) {
        CreateNode();
        nodePool_[cur].next[curSymb] = nodeCnt_;
      }
      cur = nodePool_[cur].next[curSymb];
      UpdateFrequency(cur, wordIndex);
    }
  }

  std::pair<int, std::string> SuggestWord(const std::string& prefix, int startingNode) {
    int cur = startingNode;
    for (char symb : prefix) {
      if (nodePool_[cur].next[symb - 'a'] == -1) {
        return {0, "No suggestion for this prefix!"};
      }
      cur = nodePool_[cur].next[symb - 'a'];
    }
    return {cur, wordList_[nodePool_[cur].maxFrequentIndex]};
  }
};


bool isInteger(const std::string& str) { 
    for (size_t i = 0; i < str.length(); ++i) { 
        if (!std::isdigit(str[i])) { 
            return false; 
        } 
    } 
 
    return true; 
} 

void Interaction() {
  Trie myTrie;
  std::string command;
  std::cin >> command;

  int sysVal = kRootVal;

  while (command != "end") {
    if (command == "insert") {
      sysVal = kRootVal;

      std::string toCheck;
      std::cin >> toCheck;

      while (!isInteger(toCheck)) {
        std::cout << "Try again, you need to type a positive number!\n";
        std::cin >> toCheck;
      }

      int amount = std::stoi(toCheck);

      for (int i = 0; i < amount; ++i) {
        std::string curWord;
        std::cin >> curWord;
        myTrie.InsertWord(curWord);
      }
    } else if (command == "request") {
      sysVal = kRootVal;

      std::string curPrefix;
      std::cin >> curPrefix;
      auto[newSysVal, curAnswer] = myTrie.SuggestWord(curPrefix, sysVal);

      sysVal = newSysVal;
      std::cout << "Suggestion: " << curAnswer << "\n";

    } else if (command == "add") {
      std::string adding;
      std::cin >> adding;
      auto[newSysVal, curAnswer] = myTrie.SuggestWord(adding, sysVal);

      sysVal = newSysVal;
      std::cout << "New suggestion: " << curAnswer << "\n";
    } else {
      std::cout << "Unknown command!\n";
    }
    std::cin >> command;
  } 
}

int main() {
  Interaction();
  return 0;
}