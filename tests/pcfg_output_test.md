# PCFG Output Test

This test checks that the output of the grammar rules is in standard PCFG format (LHS -> RHS [probability]) and that probabilities are normalized for each LHS.

# Steps
1. Run the madios executable with a known input corpus and parameters.
2. Capture the output.
3. Check that the grammar section contains rules in the form:
   - E27 -> believes [0.73]
   - P28 -> E27 that [0.5]
   - S -> Cindy P32 easy [1.0]
   - etc.
4. Check that for each LHS, the sum of probabilities is (approximately) 1.0.

# Example Output
E27 -> believes [0.730769]
E27 -> thinks [0.269231]
P28 -> E27 that [1]
E29 -> read [0.5]
E29 -> please [0.5]
P30 -> P28 to E29 is [1]
E31 -> Cindy [0.4]
E31 -> Jim [0.1]
E31 -> Beth [0.1]
E31 -> Joe [0.4]
P32 -> P28 E31 P30 [1]
E33 -> cat [0.3]
E33 -> cow [0.1]
E33 -> horse [0.4]
E33 -> dog [0.2]
P34 -> the E33 [1]
P35 -> to E29 [1]
E36 -> easy [0.666667]
E36 -> eager [0.333333]
P37 -> is E36 P35 [1]
S -> Cindy P32 easy [1.0]
S -> Cindy P32 tough [1.0]
S -> Pam P32 tough [1.0]
S -> Beth P28 George P30 easy [1.0]
S -> Pam P32 easy [1.0]
S -> Beth P32 tough [1.0]
S -> that Cindy P37 annoys Cindy [1.0]
S -> that P34 P37 disturbs P34 [1.0]
S -> that P34 P37 annoys P34 [1.0]
S -> that Cindy P37 bothers P34 [1.0]
S -> that P34 P37 annoys Jim [1.0]
S -> Pam P32 easy [1.0]
S -> that P34 P37 worries P34 [1.0]
S -> Cindy P28 George P30 easy [1.0]
S -> Pam P28 Pam P30 easy [1.0]
S -> Beth P32 tough [1.0]
S -> Cindy P32 easy [1.0]
S -> that P34 is tough P35 disturbs P34 [1.0]
S -> Beth P32 tough [1.0]
S -> Cindy P32 easy [1.0]

# To update the expected output:
# 1. Run: ./madios <args> > tests/expected_output.txt
# 2. Manually verify the grammar section is in PCFG format.
# 3. Use a script or test to check normalization if needed.
