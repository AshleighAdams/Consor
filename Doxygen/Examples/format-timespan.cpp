for(double i = 1.0, test; i < 10.0; i++, test = pow(i, 6.0))
	Util::Log("% = %", test, Util::FormatTimeSpan(test));
