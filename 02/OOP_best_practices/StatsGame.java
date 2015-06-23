import java.util.Scanner;

class StatsGame {

  public void startGame() {
    Scanner scanner = new Scanner(System.in);
    do {
      System.out.println("Enter a sentence. Enter empty line to end.");
      String input = scanner.nextLine();
      if(input.isEmpty()){
        scanner.close();
        return;
      }
      String[] sentences = input.split("[!?\\.]");
      if (sentences.length > 1) {
        scanner.close();
        throw new RuntimeException(
            "There is more than one sentence here!");
      }
      String[] words = input.split("\\s+");
      if (!(words[words.length - 1].endsWith(".")
          || words[words.length - 1].endsWith("?") || words[words.length - 1]
          .endsWith("!"))) {
        scanner.close();
        throw new RuntimeException(
            "A sentence should end with either !, ? or .");
      }
      int totalWlen = 0;
      for (String word : words) {
        totalWlen += word.length();
      }
      double divided = (double) totalWlen / (double) words.length;
      System.out.println("Your sentence has " + words.length
          + " words and average word has " + divided + " characters");

    }
    while(true);
  }

}
