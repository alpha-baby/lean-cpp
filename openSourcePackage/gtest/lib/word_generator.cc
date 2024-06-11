#include "word_generator.h"

namespace sandbox {
namespace {

class helloworld_generator : public word_generator {
public:
    std::string next() const {
        return "hello, world!";
    };
};
}

std::unique_ptr<word_generator> create_generator() {
    return std::make_unique<helloworld_generator>();
};
}