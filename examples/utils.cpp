#include "utils.h"

void print_frame(const std::string_view title)
{
    std::println("{:=^20}", "");
    std::println("|{: ^18}|", title);
    std::println("{:=^20}", "");
}
