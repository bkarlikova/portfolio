#include "temporaryEffect.h"

TemporaryEffect::TemporaryEffect(const json &block, string att_name) :
    Effect(block, std::move(att_name)){}

string TemporaryEffect::build_string_of_effect() const {
    return Effect::build_string_of_effect() + " temp.\n";
}

bool TemporaryEffect::is_temporary() const {
    return true;
}