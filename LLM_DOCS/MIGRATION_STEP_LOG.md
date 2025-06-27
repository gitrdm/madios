# Migration Step Log

## Step: Move BasicSymbol.cpp and BasicSymbol.h

- Moved BasicSymbol.cpp to src/BasicSymbol.cpp
- Moved BasicSymbol.h to include/madios/BasicSymbol.h
- Updated CMakeLists.txt and includes as needed
- Ran clean_build_and_test.sh after move
- Result: (pending)

---
2025-06-26: Deleted empty/obsolete EquivalenceClass.cpp from project root (never contained code). Not migrated; confirmed by git history. See REFORM_CHECKLIST.md for details.
2025-06-26: Deleted empty/obsolete EquivalenceClass.h from project root (real header already in include/madios/).
2025-06-26: Deleted empty/obsolete .cpp/.h files from project root (all real code is in src/ or include/madios/).
