package com.b07.e1;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

// MODIFED SLIGHTLY B09 :D

public class ExerciseOne {
  /** Loops to take in user input. */
  public static void main(String argv[]) throws IOException {

    // This line will create a buffered reader that reads from the system input using an input
    // stream reader
    BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(System.in));
    // input will contain the first line of input from the user
    String input = bufferedReader.readLine();


  }

  /**
   * Returns the number of spaces contained in the given string, followed by the word spaces if
   * there is more than one, and space if there is exactly one. If there are no spaces, return no
   * spaces. This method handles bad inputs, such as strings of all spaces and null.
   * 
   * @param words string input
   * @return spaces number of spaces from string input
   */
  public static String numberOfSpaces(String words) {
    int count = 0;
    String spaces = null;
    if (words != null) {
      for (int i = 0; i < words.length(); i++) {
        if (words.charAt(i) == ' ') {
          count++;
        }
      }
    }

    // Bad input where string is either null or all spaces
    if (words == null || count == words.length()) {
      spaces = "Please enter valid input";
    } else if (count == 1) {
      spaces = "1 space";
    } else {
      spaces = String.valueOf(count) + " spaces";
    }

    return spaces;
  }

  /**
   * Returns the number of characters contained in the given string, followed by the word
   * characters if it contains more than one, and character if it contains exactly one. If there
   * are no characters present, return no characters as opposed to 0 characters. This method
   * handles bad inputs such as strings of no characters, null, etc.
   * 
   * @param words string input
   * @return chars number of chars from string input
   */
  public static String numberOfCharacters(String words) {
    String chars = null;
    return chars;
  }

}
