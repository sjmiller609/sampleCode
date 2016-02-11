import components.sequence.Sequence;
import components.sequence.Sequence1L;
import components.simplereader.SimpleReader;
import components.simplereader.SimpleReader1L;
import components.simplewriter.SimpleWriter;
import components.simplewriter.SimpleWriter1L;

/**
 * A program that counts word occurrences in a given input file and outputs an
 * HTML document with a table of the words and counts listed in alphabetical
 * order.
 * 
 * @author Steven J. Miller
 * 
 */
public final class WordCount {

    // I think it is better to determine words vs separators
    // by specifying what characters should be included in words rather
    // than by defining what characters are separators.
    // If you wished to change the characters not considered separators
    // to switch to a language with different characters, for example,
    // you could simply edit the constant below, LETTERS.
    // I don't think random symbols should be considered words
    // because in many cases, such as a typed out equation, it shouldn't.
    // (e.g. 2 + 2 = 4 should be 3 words not 5)
    // This design is consistent with single-point control over change.

    /**
     * Constant LETTERS includes all characters found in words in English,
     * upper-case letters excluded, numbers included. The characters in LETTERS
     * must be in alphabetical order.
     */
    private static final char[] LETTERS = { '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
            'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
            'y', 'z' };

    // Definition of "word" for this program: any String of char
    // that includes only char from LETTERS

    /**
     * Private constructor so this utility class cannot be instantiated.
     */
    private WordCount() {
    }

    /**
     * Outputs an HTML header.
     * 
     * @param in
     *            the input stream from file
     * @param out
     *            the output stream to HTML file
     * @updates out
     * @requires <pre>
     * {@code out != null }
     * </pre>
     * @ensures <pre>
     * {A valid HTML header is written in @code out, includes name of 
     * text file (@code in) in title}
     * </pre>
     */
    private static void createHTMLHeader(SimpleReader in, SimpleWriter out) {
        out.println("<html>\n<head>");
        out.println("<title>" + in.name() + "</title>\n</head>\n<body>");
        out.println("<h2>Words Counted in " + in.name() + "</h2>\n<hr/>");

    }

    /**
     * Creates HTML table and HTML footer.
     * 
     * @param list
     *            Sequence of alphabetized words
     * @param out
     *            the output stream to HTML file
     * @updates out
     * @requires <pre>
     * {@code list is alphabetized, @code out != null }
     * </pre>
     * @ensures <pre>
     * {word counts are correct, words are in alphabetical order, 
     *  code written into @code out is valid HTML code, a valid HTML
     *  footer is written to @code out after the table code}
     * </pre>
     */
    private static void createHTMLTable(Sequence<String> list, SimpleWriter out) {
        out.println("<table border=\"1\">\n<tr>\n<th>Words</th>\n"
                + "<th>Counts</th>\n</tr>");
        int totalCount = 0;
        while (list.length() > 0) {
            int count = 1;
            while (list.length() > 1 && list.entry(0).equals(list.entry(1))) {
                count++;
                list.remove(0);
                totalCount++;
            }
            out.println("<tr>");
            out.println("<td>" + list.remove(0) + "</td>");
            out.println("<td>" + count + "</td>");
            totalCount++;
        }
        out.println("<tr>");
        out.println("<td>" + "TOTAL:" + "</td>");
        out.println("<td>" + totalCount + "</td>");
        out.println("</tr>\n</table>\n</body>\n</html>");
    }

    /**
     * Creates a String of all words that were in the file.
     * 
     * @param in
     *            the input stream from file
     * @return @code bigString
     * @requires <pre>
     * {@code in != null}
     * </pre>
     * @ensures <pre>
     * {@code bigString = ((@code in) with all characters not in @code LETTERS 
     * replaced by white-spaces),
     * @code bigString includes characters only from
     * @code LETTERS and white-spaces}
     * </pre>
     */
    private static String createString(SimpleReader in) {

        // initialize String
        String bigString = "";

        // makes a String of the lines of the stream @code in
        while (!in.atEOS()) {
            bigString = bigString.concat(in.nextLine() + " ");
        }

        // change all letters to lower-case in @code bigString
        bigString = bigString.toLowerCase();

        // replace all characters not in @code LETTERS with ' ' character:
        for (int i = 0; i < bigString.length(); i++) {
            boolean isLetter = false;
            char temp = bigString.charAt(i);
            for (int k = 0; k < LETTERS.length; k++) {
                if (temp == LETTERS[k]) {
                    isLetter = true;
                }
            }
            if (!isLetter) {
                bigString = bigString.replace(temp, ' ');
            }

        }

        return bigString;

    }

    /**
     * Creates a Sequence of all words that are in a String. Uses the method
     * firstWord(String x) in the method body
     * 
     * @param input
     *            the input String
     * @return result
     * @requires <pre>
     * {@code input contains only @code LETTERS and whitespace}
     * </pre>
     * @ensures <pre>
     * {@code result contains only characters from @code LETTERS, words in
     * @code result = words in @code input}
     * </pre>
     */
    private static Sequence<String> stringToSequence(String input) {
        Sequence<String> result = new Sequence1L<String>();
        String temp = "";

        // erases leading whitespace
        while (input.length() > 0 && input.charAt(0) == ' ') {
            input = input.substring(1);
        }

        if (!input.isEmpty()) {

            // returns first word from input
            temp = firstWord(input);

            // erases word from input
            while (input.charAt(0) != ' ') {
                input = input.substring(1);
            }

            // adds the word to the Sequence
            result.add(0, temp);

            // recursive call to add the rest of the words to the sequence
            result.append(stringToSequence(input));
        }
        return result;
    }

    /**
     * Returns just the first word present in a String.
     * 
     * @param input
     *            the input String
     * @return temp
     * @requires <pre>
     * {@code input starts with a word not white-space, @code input
     * must contain only words and white-space}
     * </pre>
     * @ensures <pre>
     * {only the first word in the String @code input is returned}
     * </pre>
     */
    private static String firstWord(String input) {
        String temp = "";

        if (input.charAt(0) != ' ') {
            temp = temp.concat(input.substring(0, 1));
            temp = temp.concat(firstWord(input.substring(1)));
        }

        return temp;
    }

    /**
     * Alphabetizes a Sequence of Strings.
     * 
     * @param wordSeq
     *            the Sequence of Strings
     * @updates wordSeq
     * 
     * @ensures <pre>
     * {@code wordSeq is alphabetical}
     * </pre>
     */
    private static void alphabetize(Sequence<String> wordSeq) {
        Sequence<String> temp = new Sequence1L<>();

        // Uses recursive call to append the words to wordSeq.
        // The most alphabetical word is appended deepest 
        // in the recursion stack.
        // The least alphabetical word is appended last.
        // (within LETTERS: 1 is most, z is least)

        if (wordSeq.length() > 0) {

            String high = wordSeq.entry(0);
            temp.add(0, wordSeq.remove(0));

            for (int i = 0; i < wordSeq.length(); i++) {
                if (high.compareTo(wordSeq.entry(i)) < 0) {
                    high = wordSeq.entry(i);
                    temp.add(0, wordSeq.remove(i));
                }
            }

            temp.remove(0);
            wordSeq.append(temp);
            alphabetize(wordSeq);
            wordSeq.add(wordSeq.length(), high);
        }

    }

    /**
     * Main method.
     * 
     * @param args
     *            the command line arguments
     */
    public static void main(String[] args) {
        SimpleReader in = new SimpleReader1L();
        SimpleWriter out = new SimpleWriter1L();

        // ask user for input file name and desired output file name
        out.println("What is the name of the input file?");
        SimpleReader infile = new SimpleReader1L(in.nextLine());
        out.println("Output file name desired:");
        String outName = in.nextLine();

        // makes sure the output file is HTML
        if (!outName.endsWith(".html")) {
            outName = outName.concat(".html");
        }
        SimpleWriter outfile = new SimpleWriter1L(outName);

        // create HTML header
        createHTMLHeader(infile, outfile);

        // create unordered String of words
        String bigString = createString(infile);

        // turn String in a sequence of words
        Sequence<String> list = stringToSequence(bigString);

        // order the sequence
        alphabetize(list);

        // make table, footer
        createHTMLTable(list, outfile);

        /*
         * Close input and output streams
         */

        infile.close();
        outfile.close();
        in.close();
        out.close();
    }
}
