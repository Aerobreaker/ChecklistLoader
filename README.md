# ChecklistLoader
 Loads a text file containing steps for a checklist so you can check through them one at a time.


### Checklist file format
 Files loaded will be read by line according to the following rules:
 1. Characters will be read until the first non-leading whitespace character
 2. These characters will be split on the period (.) character to determine step and sub-step
 3. The first set of non-leading whitespace characters will be ignored
 4. The remaining characters on the line will be read as the text for that step
 5. Leading whitespace characters will cause the relevant step to be treated as a sub-step for the preceeding step with fewer whitespace characters
 6. If there are multiple lines with the same step, only the last value will be retained

### Example checklist file
    1. Testing
      a. Test
      b. Test B
    2.a. Test C
    2.c. Test D
    2. This is a test
    3.	Test 2
 With this example file, you'll end up with a checklist similar to the following
 1. Testing
     1. Test
     2. Test B
 2. This is a test
     1. Test C
     2. Test D
 3. Test 2
