"""
Get the FX rates for a set of currencies
"""

import sys
import optparse
import requests
import typing

def get_fx_rates(currencies: list[str]) -> list[list[float]]:
    """
    Get the FX rates for a set of currencies
    """
    rates = []
    base_url = "https://api.exchangerate-api.com/v4/latest/"
    for i, ccy_s in enumerate(currencies):
        row = []
        url = f"{base_url}{ccy_s}"
        response = requests.get(url).json()["rates"]
        for j, ccy_t in enumerate(currencies):
            if i == j:
                row.append(1.0)
            elif j < i:
                row.append(1.0 / rates[j][i])
            else:
                row.append(response[ccy_t])
        rates.append(row)
    return rates

def generate_code(currencies: list[str], rates: list[list[float]], output: typing.IO):
    import datetime as d
    now = d.datetime.now();
    microseconds_since_epoch = int(now.timestamp() * 1_000_000)
    
    print(f"// FX rates from Exchange Rate API", file=output)
    print(f"// generated at {now.isoformat()}", file=output)
    print(f"#pragma once", file=output)
    print(file=output)
    print(f"#include <vector>", file=output)
    print(f"#include <string>", file=output)
    print(f"#include <chrono>", file=output)
    print(file=output)
    print(f"namespace {{", file=output)
    
    print(f"  const std::chrono::system_clock::time_point rates_date(std::chrono::microseconds({microseconds_since_epoch}LL));", file=output)

    n = len(currencies)
    ccy_list = '", "'.join(currencies)
    print(f'  std::vector<std::string> currencies {{ "{ccy_list}" }};', file=output)

    print(f"  const double rates[{n}][{n}] = {{", file=output)
    for i, row in enumerate(rates):
        print(f"     /*{currencies[i]}*/ {{ {', '.join(map(str, row))} }},", file=output)
    print(f"  }};", file=output)

    print(f"}} // namespace", file=output)
    output.close()


if __name__ == '__main__':
    parser = optparse.OptionParser()
    parser.add_option("-c", "--currencies", dest="currencies", default="USD,EUR,GBP,JPY,CHF,CNY,BRL,KRW,INR",
                      help="Comma separated list of currencies to get rates for [default: %default]")
    parser.add_option("-o", "--output", dest="output", default=None,
                      help="Output file for the rates [default: stdout]")
    (options, args) = parser.parse_args()

    currencies = options.currencies.split(',')
    out = sys.stdout if options.output is None else open(options.output, 'w')
    with out:
        print(f"Getting rates for {currencies}", file=sys.stderr)
        rates = get_fx_rates(currencies)
        generate_code(currencies, rates, out)
