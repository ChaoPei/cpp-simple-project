int main() {
	OS windows = new Windows();
	OS linux = new Linux();
	Computer asus = new Asus();
	Computer dell = new Dell();
	dell->installOS(windows);
	asus->installOS(linux);
}