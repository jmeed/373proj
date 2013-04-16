package com.example.swatch;

public class Utility {
	
	public static String mUserCity;
	public static String mUserState;
	public static String mUserZip;
	public static String mForecast;
	public static String mCurrent_cond;
	public static String mIcon_cur;
	public static String mIcon_forecast;
	public static String mTime;
	public static int mHeadline_counter;
	public static String mH0;
	public static String mH1;
	public static String mH2;
	public static String mH3;
	public static String mH4;
	public static String mH5;
	public static String mH6;
	public static String mH7;
	public static String mH8;
	public static String mH9;
	public static String mH10;
	public static String mH11;
	public static String to_send_0;
	public static String to_send_1;
	public static String to_send_2;
	public static String to_send_3;
	public static int how_many_sends_total;
	public static int how_many_sends;
	
	static String get_to_send()
	{
		String to_return = null;
		switch(how_many_sends)
		{
		case 0:
			to_return = to_send_0;
			break;
		case 1:
			to_return = to_send_1;
			break;
		case 2:
			to_return = to_send_2;
			break;
		case 3:
			to_return = to_send_3;
			break;
		}
		return to_return;
	}

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

	
	static void set_BT(String to_send)
	{
		// Resize if necessary
		if (to_send.length() > 188)
			to_send = to_send.substring(0, 188);
		// Save strings ----------------------------------------------
		int num_of_sends = (int) Math.ceil((double)to_send.length()/64.0);
		System.out.println("num_of_sends "+num_of_sends);
		how_many_sends_total = num_of_sends;
		int sub_start = 0;
		int sub_end = 64;
		
		for (int i = 0; i < num_of_sends; i++)
		{
			String sending = new String();
			
			if (sub_end > to_send.length())
				sending = to_send.substring(sub_start);
			else
				sending = to_send.substring(sub_start, sub_end);
				
			sub_start = sub_start + 64;
			sub_end = sub_end + 64;
			
			if (i == 0)
			{
				to_send_0 = sending;
				System.out.print("to_send_0: "+to_send_0);
			}
			else if (i == 1)
			{
				to_send_1 = sending;
				System.out.print("to_send_1: "+to_send_1);
			}
			else if (i == 2)
			{
				to_send_2 = sending;
				System.out.print("to_send_2: "+to_send_2);
			}
			else
			{
				to_send_3 = sending;
				System.out.print("to_send_3: "+to_send_2);
			}
		
		}
		// END save to strings -----------------------------------------------
	}
	
	static String get_headline(int head)
	{
		String to_return = null;
		switch(head)
		{
		case 0:
			to_return = mH0;
			break;
		case 1:
			to_return = mH1;
			break;
		case 2:
			to_return = mH2;
			break;
		case 3:
			to_return = mH3;
			break;
		case 4:
			to_return = mH4;
			break;
		case 5:
			to_return = mH5;
			break;
		case 6:
			to_return = mH6;
			break;
		case 7:
			to_return = mH7;
			break;
		case 8:
			to_return = mH8;
			break;
		case 9:
			to_return = mH9;
			break;
		case 10:
			to_return = mH10;
			break;
		case 11:
			to_return = mH11;
			break;
		default:
			to_return = null;
			break;
		}
		return to_return;
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
			to_return = "2";
		}
		else if (input.equalsIgnoreCase("cloudy") || input.equalsIgnoreCase("partlycloudy") || input.equalsIgnoreCase("fog") || input.equalsIgnoreCase("hazy") || input.equalsIgnoreCase("mostlycloudy"))
		{
			to_return = "0";
		}
		else if (input.equalsIgnoreCase("clear") || input.equalsIgnoreCase("mostlysunny") || input.equalsIgnoreCase("partlysunny") || input.equalsIgnoreCase("sunny"))
		{
			to_return = "4";
		}
		else if (input.equalsIgnoreCase("chancerain") || input.equalsIgnoreCase("rain") || input.equalsIgnoreCase("sleet"))
		{
			to_return = "1";
		}
		else
		{
			to_return = "3";
		}
		return(to_return);
	}
	
	
	
}
