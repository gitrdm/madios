#!/usr/bin/env python3
import sys, json, re

def normalize_grammar_string(s):
    # Remove all whitespace for robust comparison
    return re.sub(r"\s+", "", s) if isinstance(s, str) else s

def grammar_rules_set(s):
    # Split grammar string into rules, normalize whitespace, return as set
    if not isinstance(s, str):
        return set()
    rules = s.strip().split('\n')
    return set(normalize_grammar_string(rule) for rule in rules if rule.strip())

def main():
    if len(sys.argv) != 3:
        print("Usage: diff_grammar.py file1.json file2.json", file=sys.stderr)
        sys.exit(2)
    with open(sys.argv[1]) as f1, open(sys.argv[2]) as f2:
        a = json.load(f1)
        b = json.load(f2)
    a_rules = grammar_rules_set(a.get('grammar', ''))
    b_rules = grammar_rules_set(b.get('grammar', ''))
    only_in_a = a_rules - b_rules
    only_in_b = b_rules - a_rules
    if only_in_a:
        print("Rules only in actual output:")
        for rule in sorted(only_in_a):
            print(rule)
    if only_in_b:
        print("Rules only in expected output:")
        for rule in sorted(only_in_b):
            print(rule)
    if not only_in_a and not only_in_b:
        print("All grammar rules match (ignoring order and whitespace).")
        sys.exit(0)
    else:
        sys.exit(1)

if __name__ == "__main__":
    main()
