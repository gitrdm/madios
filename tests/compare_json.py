#!/usr/bin/env python3
import sys, json, math, re
from collections.abc import Mapping, Sequence

def isclose(a, b, rel_tol=1e-9, abs_tol=1e-9):
    try:
        return math.isclose(a, b, rel_tol=rel_tol, abs_tol=abs_tol)
    except Exception:
        return False

def normalize_grammar_string(s):
    # Remove all whitespace (spaces, tabs, newlines) for robust comparison
    return re.sub(r"\s+", "", s) if isinstance(s, str) else s

def normalize_grammar(grammar_str):
    # Split into lines, strip whitespace, remove empty lines, and sort
    lines = [line.strip().replace(' ', '') for line in grammar_str.strip().split('\n') if line.strip()]
    return set(lines)

def normalize_lexicon_string(s):
    # Remove all whitespace and punctuation except alphanumerics and brackets
    return re.sub(r'[^\w\[\]]', '', s)

def compare(a, b, path=None):
    if path is None:
        path = []
    # Special handling for the 'grammar' field
    if path and path[-1] == 'grammar' and isinstance(a, str) and isinstance(b, str):
        return normalize_grammar_string(a) == normalize_grammar_string(b)
    if isinstance(a, Mapping) and isinstance(b, Mapping):
        if set(a.keys()) != set(b.keys()):
            return False
        return all(compare(a[k], b[k], path + [k]) for k in a)
    elif isinstance(a, Sequence) and not isinstance(a, (str, bytes)) and \
         isinstance(b, Sequence) and not isinstance(b, (str, bytes)):
        if len(a) != len(b):
            return False
        return all(compare(x, y, path + [str(i)]) for i, (x, y) in enumerate(zip(a, b)))
    elif isinstance(a, float) and isinstance(b, float):
        return isclose(a, b)
    else:
        return a == b

def compare_lexicon(actual, expected):
    if len(actual) != len(expected):
        print(f"Lexicon length mismatch: {len(actual)} vs {len(expected)}")
        return False
    for a, e in zip(actual, expected):
        for key in a:
            if key == 'string':
                if normalize_lexicon_string(a[key]) != normalize_lexicon_string(e[key]):
                    print(f"Mismatch in lexicon string: '{a[key]}' vs '{e[key]}'")
                    return False
            else:
                if a[key] != e[key]:
                    print(f"Mismatch in lexicon field '{key}': {a[key]} vs {e[key]}")
                    return False
    return True

def compare_json(actual, expected):
    # Ignore timing field if present
    if isinstance(actual, dict) and isinstance(expected, dict):
        actual = dict(actual)
        expected = dict(expected)
        actual.pop("timing", None)
        expected.pop("timing", None)
    # Compare all fields except 'grammar' and 'lexicon'
    for key in actual:
        if key in ('grammar', 'lexicon'):
            continue
        if key not in expected or actual[key] != expected[key]:
            print(f"Mismatch in field '{key}'!")
            print(f"Actual:   {actual.get(key)}")
            print(f"Expected: {expected.get(key)}")
            return False
    for key in expected:
        if key in ('grammar', 'lexicon'):
            continue
        if key not in actual:
            print(f"Missing field '{key}' in actual output!")
            return False

    # Compare grammar as sets of stripped lines
    actual_grammar = normalize_grammar(actual['grammar'])
    expected_grammar = normalize_grammar(expected['grammar'])
    if actual_grammar != expected_grammar:
        print("Grammar rules differ!")
        print("--- Actual ---")
        print('\n'.join(sorted(actual_grammar)))
        print("--- Expected ---")
        print('\n'.join(sorted(expected_grammar)))
        return False

    # Compare lexicon robustly
    if not compare_lexicon(actual['lexicon'], expected['lexicon']):
        print("Lexicon objects differ!")
        print("Actual:", actual['lexicon'])
        print("Expected:", expected['lexicon'])
        return False

    return True

def main():
    if len(sys.argv) != 3:
        print("Usage: compare_json.py file1.json file2.json", file=sys.stderr)
        sys.exit(2)
    with open(sys.argv[1]) as f1, open(sys.argv[2]) as f2:
        a = json.load(f1)
        b = json.load(f2)

        compare_json(a, b)

if __name__ == "__main__":
    main()
