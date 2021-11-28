//#pragma debug

// get html from https://www.unicode.org/charts/
// parse the unicode ranges to identify the names of each valid range
// sort the ranges and filter out overlapping ranges
// build the c++ output

void main()
{
	// Download the html code
	string url = "https://www.unicode.org/charts/";
	print("Downloading from '" + url + "'. Please wait...\n");
	string result;
	exec("curl -s " + url, result);
	print("Download completed\n");
	
	// Add the latin and latin-1 supplement ranges as a single range to keep them together 
	array<Range@> ranges;
//	ranges.insertLast(Range("0000-00FF", "Latin + Latin-1 Supplement"));
	
	// Parse the html code to identify the valid ranges
	int p = 0;
	for(;;)
	{
		p = result.findFirst("<p class=\"", p);
		if( p == -1 )
			break;
		
		string cl = result.substr(p+10,2);
		if( cl == "mb" || cl == "sb" || cl == "pb" )
		{
			// Most cases look like: <p class="mb"><a href="PDF/U0530.pdf" title="0530-058F">Armenian</a></p>
			// special case? <p class="mb">Game Symbols</p>
			// special case: <p class="pb"><a href="PDF/U0000.pdf">Basic operators: Plus, Factorial,</a> <a href="PDF/U0080.pdf">Division, Multiplication</a></p>
			// special case: <p class="pb"><a href="PDF/U0000.pdf" title="0024">Dollar Sign</a>, <a href="PDF/U20A0.pdf" title="20AC">Euro Sign</a></p>
			// special case: <p class="pb"><a href="PDF/U2600.pdf" title="2654-265F, 26C0-26C3">Chess, Checkers/Draughts</a></p>
			// special case: <p class="pb"><a href="PDF/U2600.pdf" title="268A-268F, 2630-2637">Yijing Mono-, Di- and Trigrams</a></p>
			// special case: <p class="pb">Controls:	<a href="PDF/U0000.pdf" title="0000-001F">C0</a>,	<a href="PDF/U0080.pdf" title="0080-009F">C1</a></p>
			
			// Find </p> to get the limit of the paragraph
			int end = result.findFirst("</p>", p);
			string line = result.substr(p, end - p);
			
			// Look for the range and name
			// There can be multiple range/name pairs
			int p1 = 0;
			for(;;)
			{
				p1 = line.findFirst("title=\"", p1+1);
				if( p1 < 0 )
					break;

				int p2 = line.findFirst("\"", p1+7);
				string range = line.substr(p1+7, p2-p1-7);
				p1 = line.findFirst("</a>", p2);
				string name = line.substr(p2+2, p1-p2-2);
				
				// Split multiple ranges
				for(;;)
				{
					p2 = range.findFirst(",");
					if( p2 < 0 )
					{
						//print(range + " = " + name + "\n");
						ranges.insertLast(Range(range,name));
						break;
					}
					else
					{
						string range1 = range.substr(0, p2);
						//print(range1 + " = " + name + "\n");
						ranges.insertLast(Range(range,name));
					
						range = range.substr(p2+1);
					}
				}
			}
		}

		// skip to next paragraph
		p += 10;
	}
	print("Parsing completed with " + ranges.length() + " ranges found\n");
	
	// Sort the ranges
	ranges.sort(function(a, b) { if( a.start == b.start ) return a.end > b.end; else return a.start < b.start; });
	
	// Remove overlapping ranges and determine longest name
	uint longestName = 0;
	for( uint n = 0; n < ranges.length() - 1; n++ )
	{
		while( (n < ranges.length() - 1) && ranges[n+1].start <= ranges[n].end )
		{
			//print("Removed " + ranges[n+1].start + "-" + ranges[n+1].end + " = " + ranges[n+1].name + "\n");
			ranges.removeAt(n+1);
		}
		
		if( ranges[n].name.length() > longestName )
			longestName = ranges[n].name.length();
	}
	print("Removed overlapping ranges. Remaining " + ranges.length() + "\n");
	
	// Write the C++ code
	print("Generating C++ code for copy and paste into unicode.cpp\n");
	print("---\n");
	print("const UnicodeSubset_t UnicodeSubsets[] = {\n");
	uint last = 0;
	for( uint n = 0; n < ranges.length(); n++ )
	{
		if( last + 1 < ranges[n].start )
			print('{"' + '{undefined}' + '"' + ('                                                  '.substr(0, longestName - 11)) + ', 0x' + formatUInt(last+1, '0H', 6) + ', 0x' + formatUInt(ranges[n].start-1, '0H', 6) + "}, // " + ((ranges[n].start-1) - (last+1) + 1) + "\n");
		print('{"' + ranges[n].name + '"' + ('                                                  '.substr(0, longestName - ranges[n].name.length())) + ', 0x' + formatUInt(ranges[n].start, '0H', 6) + ', 0x' + formatUInt(ranges[n].end, '0H', 6) + "}, // " + (ranges[n].end - ranges[n].start + 1) + "\n");
		last = ranges[n].end;
	}
	print("};\n");
	
	print("---\n");
	print("Press enter to exit\n");
	getInput();
}


class Range
{
	Range(string range, string name)
	{
		// Two different chars are used for dash
		int p = range.findFirst("-");
		if( p < 0 )
		{
			// unicode char utf8 0xe28093
			p = range.findFirst("–");
			if( p < 0 )
				start = end = parseUInt(trim(range), 16);
			else
			{
				start = parseUInt(trim(range.substr(0, p)), 16);
				end = parseUInt(trim(range.substr(p+3)), 16);
			}
		}
		else
		{
			start = parseUInt(trim(range.substr(0, p)), 16);
			end = parseUInt(trim(range.substr(p+1)), 16);
		}
		
		p = name.findFirst("&nbsp;");
		if( p >= 0 )
			name = name.substr(0, p) + " " + name.substr(p+6);
		p = name.findFirst("&#39;");
		if( p >= 0 )
			name = name.substr(0, p) + "'" + name.substr(p+5);
		
		this.name = trim(name);
	}

	uint start;
	uint end;
	string name;
}

string trim(string t)
{
	int p1 = t.findFirstNotOf(" \t\r\n");
	int p2 = t.findLastNotOf(" \t\r\n");
	if( p1 < 0 || p2 < 0 )
		return "";
	return t.substr(p1, p2-p1+1);
}
