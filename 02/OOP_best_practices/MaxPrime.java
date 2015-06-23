public class MaxPrime {

	public int maxPrime(int[] table) {
		checkIsOk(table);
		int x = 0;
		for (int y : table) {
			if (y > x && prime(y)) {
				x = y;
			}
		}
		return x;
	}

	private void checkIsOk(int[] list) {
		if (list.length < 1) {
			throw new SizeException();
		}
		for (int n : list) {
			if (n < 0) {
				throw new BadValue();
			}
		}
	}

	private boolean prime(int x) {
		// code to calculate if x is prime
	}

}
