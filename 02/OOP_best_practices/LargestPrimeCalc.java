public class LargestPrimeCalc {

	public int calculateLargestPrimeNumber(int[] numbers) {
		validateContainsNaturalNumbers(numbers);
		int largestPrime = 0;
		for (int current : numbers) {
			if (current > largestPrime && isPrimeNumber(current)) {
				largestPrime = current;
			}
		}
		return largestPrime;
	}

	private void validateContainsNaturalNumbers(int[] numbers) {
		if (numbers.length < 1) {
			throw new EmptyArrayException();
		}
		for (int current : numbers) {
			if (current < 0) {
				throw new NegativeValueException();
			}
		}
	}

	private boolean isPrimeNumber(int number) {
		// code to calculate if number is prime
	}

}
