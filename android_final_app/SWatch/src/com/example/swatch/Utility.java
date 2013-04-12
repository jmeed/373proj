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
		
		//int num_lines = 0;
		//num_lines = (int) Math.ceil(((double)to_be_parsed.length()/18.0));
		//System.out.println("num of lines"+num_lines);
		int sub_start = 0;
		int sub_end = 18;
		String parsed = "";
		char buf[] = new char[18];
		int i_space = 18;
		int l_space = 0;
		boolean last_line = false;
		
		System.out.println("size of weather "+to_be_parsed.length());
		
		while (true)
		{
			StringBuilder s = new StringBuilder(19);
			
			
			if (sub_end >= to_be_parsed.length())
			{
				last_line = true;
				sub_end = to_be_parsed.length() - 1;
			}
				
			to_be_parsed.getChars(sub_start, sub_end, buf, 0);
			for (int x=0; x<18;x++)
				System.out.println(buf[x]);
			
			
			i_space = 17;
			System.out.println("ispace before "+i_space);
			if (!last_line)
			{
				while (buf[i_space] != ' ')
				{
					System.out.println("new ispace "+i_space +"	" +buf[i_space]);
					// Check if i_space is at beginning of string
					if (i_space == 0)
					{
						i_space = 17;
						break;
					}
					i_space--;
				}
				sub_end = i_space + sub_start;
				System.out.println("NEW END "+sub_end);
			}
			// Get rid of leading spaces
			l_space = 0;
			while (buf[l_space] == ' ')
			{
				l_space++;
			}
			sub_start = sub_start + l_space;
			
			// Last line
			if (last_line)
			{
				sub_end = to_be_parsed.length() - 1;
			}
			
			System.out.println("sub_end set "+sub_end);
				
			
			s = s.append(to_be_parsed.substring(sub_start, sub_end) + '\n');
			System.out.println(to_be_parsed.substring(sub_start, sub_end));
			
			
			sub_start = sub_end;
			sub_end = sub_end + 18;
			parsed = parsed + s.toString();
			System.out.println("parsed size " + parsed.length());
			if (last_line)
				break;
		}
		return parsed;
}

	
}
