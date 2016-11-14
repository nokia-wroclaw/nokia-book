#pragma once

#include <string>
#include <vector>

struct Asm {
    struct Block {
        std::string label;
        std::vector<std::string> asmDirectives;

        std::string getLabel() const {
            return label;
        }

        void addAsm(std::string asmStr) {
            asmDirectives.push_back(asmStr);
        }

        void addInstr(std::string asmInstr) {
            addAsm("    " + asmInstr);
        }
    };

    unsigned label;
    const std::string labelPrefix;
    typedef std::vector<Block> Blocks;
    Blocks blocks;

    Asm() : label(0), labelPrefix(".L") {}

    int getNextLabel() {
        return label++;
    }

    void addBlock(Block block) {
        blocks.push_back(block);
    }

    Block createBlockWithLabel(std::string label = "") {
        if (not label.empty())
            return Block{label, {}};
        else
            return Block{labelPrefix + std::to_string(getNextLabel()), {}};

    }

    Block createBlockWithoutLabel() {
        return Block{"", {}};
    }

    void createAndAddBlockWithLabel(
            std::vector<std::string> directives,
            std::string label = "") {
        auto block = createBlockWithLabel(label);
        for (auto directive: directives)
            block.addInstr(directive);
        addBlock(block);
    }

    void createAndAddBlockWithoutLabel(std::vector<std::string> directives) {
        auto block = createBlockWithoutLabel();
        for (auto directive: directives)
            block.addInstr(directive);
        addBlock(block);
    }

    std::string print() const {
        std::string ret;
        for (auto asmBlock : blocks) {
            if (not asmBlock.getLabel().empty())
                ret += asmBlock.getLabel() + ":\n";
            for (auto asmDir: asmBlock.asmDirectives)
                ret += asmDir + "\n";
        }

        return ret;
    }
};

