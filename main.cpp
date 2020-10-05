

#include <iostream>

#include "meta_filter.h"

int main(void)
{
    struct tau_tag;
    constexpr auto tau = variable<tau_tag>{}; // tau = RC

    auto filter = make_filter<float>(1/(1 + tau * s));

    filter.set_variable(T, 1.f);
    filter.set_variable(tau, 20.f);

    for (auto i = 0; i < 100; i++) {
        const auto out = filter.process_one_sample(i < 50 ? 1.f : 0.f);

        const auto len = static_cast<int>(100.0f * out);
        for (auto j = 0; j < len; ++j)
            std::cout << "|";
        std::cout << std::endl;
    }

    const auto out = filter.process_one_sample(0.0);


    //  out is not optimized away
    return (out < 0.2321);
}

