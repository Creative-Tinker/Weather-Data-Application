#include "variant-selector.h"

void VariantSelection::init(std::string _name, std::vector<std::string> _variants) {
    name = _name;
    variants = _variants;
    if (variants.empty())
        throw std::runtime_error("variant list cannot be empty");
    selected = variants[0];
}

bool VariantSelection::includes(std::string needle) {
    return std::end(variants) != std::find(std::begin(variants), std::end(variants), needle);
}

void VariantSelection::try_modify(std::string value) {
    if (value.empty())
        return;
    if (includes(value))
        selected = value;
    else
        throw std::runtime_error("Invalid value \"" + value + "\" for key \"" + name +
                                 "\". Accepted values: " + fmt_list());
}

std::string VariantSelection::fmt_list() {
    return "{ " + join_many_to_one_str(variants, ", ") + " }";
}
