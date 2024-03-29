#include "permanentEffect.h"

#include <utility>

PermanentEffect::PermanentEffect(const json &block, string att_name) : Effect(block, std::move(att_name)) {}

string PermanentEffect::build_string_of_effect() const {
    return Effect::build_string_of_effect() + " perm.\n";
}

bool PermanentEffect::is_permanent() const {
    return true;
}