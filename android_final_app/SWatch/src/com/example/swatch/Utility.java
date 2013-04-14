package com.example.swatch;

public class Utility {
	
	public static String mUserCity;
	public static String mUserState;
	public static String mUserZip;
	public static String mNumHeadlines;

	static String parse_string(String to_be_parsed)
	{
		// Check string
		if (to_be_parsed.length() < 19)
			return to_be_parsed;
		
		// START parsing ------------------------------------
		int sub_start = 0;
		int sub_end = 18;
		String parsed = "";
		char buf[] = new char[18];
		int i_space = 18;
		int l_space = 0;
		boolean last_line = false;
		
		System.out.println("to_be_parsed size: "+to_be_parsed.length());
		
		while (true)
		{
			StringBuilder s = new StringBuilder(19);
			
			
			if (sub_end >= to_be_parsed.length())
			{
				last_line = true;
				sub_end = to_be_parsed.length() - 1;
			}
			
			// Get char array of to_be_parsed
			to_be_parsed.getChars(sub_start, sub_end, buf, 0);
			
			// Last index of row on screen is 17
			i_space = 17;
			if (!last_line)
			{
				while (buf[i_space] != ' ')
				{
					// Check if no spaces in line
					if (i_space == 0)
					{
						i_space = 17;
						break;
					}
					i_space--;
				}
				sub_end = i_space + sub_start;
			}
			
			// Get rid of leading spaces
			l_space = 0;
			while (buf[l_space] == ' ')
			{
				l_space++;
			}
			sub_start = sub_start + l_space;
			
			// Last line: set end appropriately
			if (last_line)
			{
				sub_end = to_be_parsed.length() - 1;
			}
			
			// Append newline to end
			s = s.append(to_be_parsed.substring(sub_start, sub_end) + '\n');
			
			sub_start = sub_end;
			sub_end = sub_end + 18;
			parsed = parsed + s.toString();
			
			if (last_line)
				break;
		}
		return parsed;
	}

	
	static void send_over_BT(String to_send)
	{
		// Send over Bluetooth ----------------------------------------------
		int num_of_sends = (int) Math.ceil((double)to_send.length()/64.0);
		System.out.println("num_of_sends "+num_of_sends);

		int sub_start = 0;
		int sub_end = 64;
		
		for (int i = 0; i < num_of_sends; i++)
		{
			String sending = new String();
						
			System.out.println("Printing 64 "+sub_start+" " + sub_end + " " + to_send.length());
			if (sub_end > to_send.length())
				sending = to_send.substring(sub_start);
			else
				sending = to_send.substring(sub_start, sub_end);
				
			sub_start = sub_start + 64;
			sub_end = sub_end + 64;
			
			// Actually send data
			CommThread.write(sending.getBytes());
			try {
				Thread.sleep(10000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		// END send to Bluetooth -----------------------------------------------
			
	  	// Sleep so processor can get string
		try {
			Thread.sleep(10000);
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

	}
	
	static int get_headline_lastindex(int start_index, String input)
	{
		int end_index = start_index;
		for (end_index = start_index; end_index < input.length(); end_index++)
		{
			if (input.charAt(end_index) == '\0')
				break;
			
		}
		return end_index + 1;
	}
	// cloudy = 1, snowy = 0, stormy = 4, sunny = 2, rain = 3
	static String choose_pic(String input)
	{
		String to_return = "nothing";
		System.out.println("input "+input);
		if (input.equalsIgnoreCase("chanceflurries") || input.equalsIgnoreCase("chancesnow") || input.equalsIgnoreCase("flurries") || input.equalsIgnoreCase("snow"))
		{
			to_return = "0";
		}
		else if (input.equalsIgnoreCase("cloudy") || input.equalsIgnoreCase("partlycloudy") || input.equalsIgnoreCase("fog") || input.equalsIgnoreCase("hazy") || input.equalsIgnoreCase("mostlycloudy"))
		{
			to_return = "1";
		}
		else if (input.equalsIgnoreCase("clear") || input.equalsIgnoreCase("mostlysunny") || input.equalsIgnoreCase("partlysunny") || input.equalsIgnoreCase("sunny"))
		{
			to_return = "2";
		}
		else if (input.equalsIgnoreCase("chancerain") || input.equalsIgnoreCase("rain") || input.equalsIgnoreCase("sleet"))
		{
			to_return = "3";
		}
		else
		{
			to_return = "4";
		}
		return(to_return);
	}
	
	
	
}
