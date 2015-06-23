import java.util.Scanner;

class SentenceStatsGame {

	private Talkative userIO;

	private static final String PROMPT_MESSAGE = "Enter a sentence. Enter empty line to end.";

	public void startGame() {
		userIO.openChannel();
		try {
			playGame();
		} finally {
			userIO.closeChannel();
		}
	}

	private void playGame() {
		do {
			playUntilPlayerEndedOrError();
		} while (true);
	}

	private void playUntilPlayerEndedOrError() {
		userIO.send(PROMPT_MESSAGE);
		String sentence = userIO.getInput();
		if (sentence.isEmpty()) {
			return;
		}
		String result = obtainStats(sentence);
		userIO.send(result);
	}

	private String obtainStats(String input) {
		SentenceProcessor processor = new SentenceProcessor(input);
		processor.validateSentence();
		int wordCount = processor.getWordCount();
		double wordLength = processor.calculateAverageWordLength();
		return formatStats(wordCount, wordLength);
	}

	private String formatStats(double wordCount, double wordLength) {
		return "Your sentence has " + wordCount
				+ " words and an average word has " + wordLength
				+ " characters";
	}

}

class SentenceProcessor {

	private static final String VALID_END_OF_SENTENCE = "[!?\\.]";
	private String sentence;
	private String[] words;

	public SentenceProcessor(String sentence) {
		this.sentence = sentence;
		words = makeWords();
	}

	public void validateSentence() {
		validateNotMoreThanOneSentence();
		validateSentenceEnding(words);
	}

	private String[] makeWords() {
		String[] words = sentence.split("\\s+");
		return words;
	}

	private void validateSentenceEnding(String[] words) {
		String lastWord = words[words.length - 1];
		String lastChar = lastWord.substring(lastWord.length() - 1);
		if (!(lastChar.matches(VALID_END_OF_SENTENCE))) {
			throw new InvalidEndOfSentenceException();
		}
	}

	private void validateNotMoreThanOneSentence() {
		String[] sentences = sentence.split(VALID_END_OF_SENTENCE);
		if (sentences.length > 1) {
			throw new TooManySentencesException();
		}
	}

	public int getWordCount() {
		return words.length;
	}

	public double calculateAverageWordLength() {
		int totalWordsLength = 0;
		for (String word : words) {
			totalWordsLength += word.length();
		}
		return (double) totalWordsLength / (double) words.length;
	}
}

class InvalidEndOfSentenceException extends RuntimeException {
	public InvalidEndOfSentenceException() {
		super("A sentence should end with either !, ? or .");
	}
}

class TooManySentencesException extends RuntimeException {
	public TooManySentencesException() {
		super("There is more than one sentence here!");
	}
}

interface Talkative {
	void openChannel();

	String getInput();

	void send(String message);

	void closeChannel();
}

class ConsoleIO implements Talkative {

	Scanner scanner;

	@Override
	public void openChannel() {
		scanner = new Scanner(System.in);
	}

	@Override
	public String getInput() {
		return scanner.nextLine();
	}

	@Override
	public void send(String message) {
		System.out.println(message);
	}

	@Override
	public void closeChannel() {
		scanner.close();
	}

}