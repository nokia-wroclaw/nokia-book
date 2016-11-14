#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>


const int WordSize = 4;

struct FunctionDef {
    std::string name;
    std::string symName;
    std::size_t numberOfArgs;
    std::size_t frameSize;
    bool predefined;
};

class StoreManager {
    struct Frame {
        FunctionDef func;
        int argsOffset;
        int varsOffset;
        std::vector<std::pair<std::string, int>> vars;

        Frame(FunctionDef func)
            : func(func)
            , argsOffset((func.numberOfArgs + 1) * WordSize)
            , varsOffset(- WordSize)
        { }
    };

    std::unique_ptr<Frame> frame;

public:
    StoreManager() : frame(nullptr) {}

    void newFunction(FunctionDef func) {
        frame.reset(new Frame(func));
    }

    void pushLocalVariable(std::string id) {
        frame->vars.push_back({id, frame->varsOffset});
        frame->varsOffset -= WordSize;
        assert(frame->varsOffset + WordSize <= static_cast<int>(frame->func.frameSize));
    }

    void popLocalVariable(std::string id) {
        assert(not frame->vars.empty());
        assert(frame->vars.back().first == id);
        frame->vars.pop_back();
        frame->varsOffset += WordSize;
    }

    void addArgumentVariable(std::string id) {
        frame->vars.push_back({id, frame->argsOffset});
        frame->argsOffset -= WordSize;
    }

    int getVariableOffset(std::string id) {
        auto currentVars = frame->vars;
        auto var =
            std::find_if(
                currentVars.cbegin(),
                currentVars.cend(),
                [id](decltype(currentVars)::value_type const &var)
                    { return var.first == id; });
        return var->second;
    }
};

