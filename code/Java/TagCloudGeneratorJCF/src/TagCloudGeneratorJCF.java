import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Queue;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

/**
 * Program that generates a tag cloud from a given input text into a well
 * formatted HTML document using JCF.
 * 
 * @author Chaz Gordish
 * @author Steven Miller
 * 
 */
public final class TagCloudGeneratorJCF {
    /**
     * Constants: maximum and minimum font size for the Tag Cloud.
     */
    private static final Integer MAX_FONT = 48, MIN_FONT = 11;
    /**
     * Constant: sequence of characters that are considered separators.
     */
    private static final String SEPARATORS = " \t\n\r,-.!?[]';:/()";

    /**
     * Private constructor so this utility class cannot be instantiated.
     */
    private TagCloudGeneratorJCF() {
    }

    /**
     * Returns the first "word" (maximal length string of characters not in
     * {@code separators}) or "separator string" (maximal length string of
     * characters in {@code separators}) in the given {@code text} starting at
     * the given {@code position}.
     * 
     * @param text
     *            the {@code String} from which to get the word or separator
     *            string
     * @param position
     *            the starting index
     * @param separators
     *            the {@code Set} of separator characters
     * @return the first word or separator string found in {@code text} starting
     *         at index {@code position}
     * @requires <pre>
     * {@code 0 <= position < |text|}
     * </pre>
     * @ensures <pre>
     * {@code nextWordOrSeparator =
     *   text[position, position + |nextWordOrSeparator|)  and
     * if entries(text[position, position + 1)) intersection separators = {}
     * then
     *   entries(nextWordOrSeparator) intersection separators = {}  and
     *   (position + |nextWordOrSeparator| = |text|  or
     *    entries(text[position, position + |nextWordOrSeparator| + 1))
     *      intersection separators /= {})
     * else
     *   entries(nextWordOrSeparator) is subset of separators  and
     *   (position + |nextWordOrSeparator| = |text|  or
     *    entries(text[position, position + |nextWordOrSeparator| + 1))
     *      is not subset of separators)}
     * </pre>
     */
    private static String nextWordOrSeparator(String text, int position,
            Set<Character> separators) {
        assert text != null : "Violation of: text is not null";
        assert separators != null : "Violation of: separators is not null";
        assert 0 <= position : "Violation of: 0 <= position";
        assert position < text.length() : "Violation of: position < |text|";

        String textIndex = text.substring(position);
        int count = 0;
        boolean test = separators.contains(textIndex.charAt(count));
        count++;
        while (count < textIndex.length()
                && test == separators.contains(textIndex.charAt(count))) {
            count++;
        }
        return textIndex.substring(0, count);
    }

    /**
     * Reports the map of Words and their count of the times it appears in the
     * file.
     * 
     * @param input
     *            file containing words
     * 
     * @param elements
     *            The characters used to separate the words
     * 
     * @return Map of Words and the number of times the words appear in the file
     * @requires <pre>
     * The given file is a valid document containing words.
     * </pre>
     * @ensures <pre>
     * The map contains all the words from the file as keys and the 
     * corresponding number as to the amount of times the word appears in the
     * file.
     * </pre>
     */
    private static Map<String, Integer> getWords(BufferedReader input,
            Set<Character> elements) {
        Map<String, Integer> wordMap = new HashMap<>();
        try {
            while (input.ready()) {
                String line = input.readLine();
                if (!(line == null)) {
                    // this line entered so capitalization will not be considered
                    line = line.toLowerCase();

                    for (int pos = 0; pos < line.length();) {
                        String word = nextWordOrSeparator(line, pos, elements);
                        if (!wordMap.containsKey(word)
                                && !elements.contains(word.charAt(0))) {
                            wordMap.put(word, 1);

                        } else if (wordMap.containsKey(word)) {
                            int updateCount = wordMap.get(word) + 1;
                            wordMap.put(word, updateCount);
                        }
                        pos += word.length();
                    }

                }
            }
        } catch (IOException e) {
            System.err.println("Error reading from file");
        }

        return wordMap;

    }

    /**
     * Sorts the given {@code Queue<Entry<String,Integer>>} sorted by value.
     * 
     * @param entries
     *            the {@code Queue} to sort by value
     * 
     * @updates entries
     * @requires <pre>
     * {#entries is not null}
     * </pre>
     * @ensures <pre>
     * {@code perms(entries, #entries)  and
     * entries is sorted}
     * </pre>
     */
    private static void sortByOccurrence(
            Queue<Map.Entry<String, Integer>> entries) {

        if (entries.size() > 1) {
            Entry<String, Integer> front = entries.remove();
            sortByOccurrence(entries);
            insertInOrder(entries, front);
        }

    }

    /**
     * Inserts the given {@code Entry<String,Integer>} in the
     * {@code Queue<Entry<String,Integer>>} sorted by value.
     * 
     * @param entries
     *            the {@code Queue} to insert into
     * @param entry
     *            the {@code Entry<String,Integer>} to insert
     * 
     * @updates entries
     * @requires <pre>
     * {#entries is sorted, entry is not null}
     * </pre>
     * @ensures <pre>
     * {@code perms(entries, #entries * <entry>)  and
     * entries is sorted}
     * </pre>
     */
    private static void insertInOrder(Queue<Entry<String, Integer>> entries,
            Entry<String, Integer> entry) {

        int startSize = entries.size();
        if (startSize > 0) {
            for (int i = 0; i < startSize; i++) {
                Entry<String, Integer> front = entries.remove();
                if (entries.size() == startSize - 1
                        && front.getValue() < entry.getValue()) {
                    entries.add(entry);
                }
                entries.add(front);

            }
        }
        if (startSize == entries.size()) {
            entries.add(entry);
        }
    }

    /**
     * Creates HTML header and outputs to @code outFile.
     * 
     * @param n
     *            number of words in tag cloud
     * @param outFile
     *            the output stream to HTML file
     * @param input
     *            the name of the input file
     * @updates outFile
     * @requires <pre>
     * {@code outFile != null,@code input!=null,@code n >= 0}
     * </pre>
     * @ensures <pre>
     * {a valid HTML header and beginning of body is written to @code outFile}
     * </pre>
     */
    private static void printHeader(int n, PrintWriter outFile, String input) {

        outFile.println("<html>\n" + "<head>\n" + "<title>Top " + n
                + " words in " + input + "</title>\n"
                + "<link href=\"tagcloud.css\" rel=\"stylesheet\" "
                + "type=\"text/css\">\n</head>\n" + "<body>\n" + "<h2>Top " + n
                + " words in " + input + "</h2>\n" + "<hr>\n"
                + "<div class=\"cdiv\">\n" + "<p class=\"cbox\">");

    }

    /**
     * Creates HTML body contents and outputs to @code outFile.
     * 
     * @param outFile
     *            the output stream to HTML file
     * @param sortAlphabetize
     *            contains pairs with key values corresponding to words and
     *            values corresponding to the word counts
     * @updates outFile
     * @requires <pre>
     * {sortAlphabetize is sorted alphabetically AND is not null,
     * @code outFile != null}
     * </pre>
     * @ensures <pre>
     * {valid HTML body contents are written to @code outFile}
     * </pre>
     */
    private static void printBody(TreeMap<String, Integer> sortAlphabetize,
            PrintWriter outFile) {

        int most = 0;
        int least = Integer.MAX_VALUE;
        /*
         * the Sorted entries are moved into a Queue in order to find the
         * largest and smallest quantities of Integer in the Pairs so that the
         * fonts may be scaled from MIN_FONT to MAX_FONT.
         */
        Queue<Map.Entry<String, Integer>> list = new LinkedList<>();

        for (Map.Entry<String, Integer> entry : sortAlphabetize.entrySet()) {
            if (entry.getValue() > most) {
                most = entry.getValue();
            }
            if (entry.getValue() < least) {
                least = entry.getValue();
            }
            list.add(entry);
        }

        while (list.size() > 0) {
            Map.Entry<String, Integer> temp = list.remove();
            int font = MAX_FONT;
            if (most > least) {
                //scale count to max/min fonts
                font = (MIN_FONT * (most - least) + (MAX_FONT - MIN_FONT)
                        * (temp.getValue() - least))
                        / (most - least);
            }
            outFile.println("<span style=\"cursor:default\" class=\"f" + font
                    + "\" title=\"count: " + temp.getValue() + "\">"
                    + temp.getKey() + "</span>");
        }
    }

    /**
     * Creates HTML footer and outputs to @code outFile.
     * 
     * @param outFile
     *            the output stream to HTML file
     * 
     * @updates outFile
     * @requires @code outFile != null
     * @ensures <pre>
     * {a valid HTML footer is written to @code outFile}
     * </pre>
     */
    private static void printFooter(PrintWriter outFile) {
        outFile.println("</p>\n</div>\n</body>\n</html>");
    }

    /**
     * Main method.
     * 
     * @param args
     *            the command line arguments
     */
    public static void main(String[] args) {
        /*
         * Generate set of elements from string to define whitespace.
         */
        Set<Character> strSet = new TreeSet<>();
        for (int i = 0; i < SEPARATORS.length(); i++) {
            strSet.add(SEPARATORS.charAt(i));
        }
        /*
         * declare i/o streams
         */
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        PrintWriter out = new PrintWriter(System.out, true);
        BufferedReader inFile;
        PrintWriter outFile;
        /*
         * Prompt user for input file until an existing name is given.
         */
        String input = "";
        boolean validInput;
        do {
            validInput = true;
            out.println("Enter a file name to generate a tag cloud: ");
            try {
                input = in.readLine();
            } catch (IOException e2) {
                out.println("Invalid input: IOException thrown...");
                validInput = false;
            }
            try {
                inFile = new BufferedReader(new FileReader(input));
            } catch (FileNotFoundException e2) {
                out.println("The file: " + input + " could not be found...");
                inFile = null;
                validInput = false;
            }
        } while (!validInput);
        /*
         * Prompt user for output file.
         */
        String output = "";
        do {
            validInput = true;
            out.println("Enter a valid name for the output file: ");
            try {
                output = in.readLine();
                outFile = new PrintWriter(new BufferedWriter(new FileWriter(
                        output)));
            } catch (IOException e1) {
                out.println("An output file with the name <" + output
                        + "> could not be created...");
                outFile = null;
                validInput = false;
            }
        } while (!validInput);
        /*
         * Prompt user for number of words to use in tag cloud and convert the
         * string to an integer, checking that it is a valid input.
         */
        int n = 0;
        String numberOfWords = "";

        do {
            validInput = true;
            out.println("Enter the number of words to be "
                    + "included in the tag cloud: ");
            try {
                numberOfWords = in.readLine();
            } catch (IOException e) {
                validInput = false;
            }
            try {
                n = Integer.parseInt(numberOfWords);
            } catch (NumberFormatException e) {
                validInput = false;
            }
            if (n < 0) {
                validInput = false;
            }
            if (!validInput) {
                out.println("The input: <"
                        + numberOfWords
                        + "> is not a valid positive integer input [0,2^31-1]...");
            }
        } while (!validInput);

        /*
         * Call to method to produce a map of each word and its count.
         */
        Map<String, Integer> words = getWords(inFile, strSet);

        /*
         * Make sure that there are enough different words. If not, sets n to be
         * equal to the number of words in the file and sends a message to the
         * console
         */
        if (n > words.size()) {
            out.println("There are less than " + n + " discrete words in "
                    + input + ".\nOnly " + words.size()
                    + " words will be in the tag cloud.");
            n = words.size();
        }
        /*
         * sort words by occurrence.
         */
        Set<Entry<String, Integer>> entrySet = words.entrySet();
        Queue<Map.Entry<String, Integer>> sortedWords = new LinkedList<>();
        for (Entry<String, Integer> x : entrySet) {
            sortedWords.add(x);
        }
        sortByOccurrence(sortedWords);
        /*
         * take the top n words
         */
        Queue<Map.Entry<String, Integer>> topNWords = new LinkedList<>();
        for (int i = 0; i < n; i++) {
            topNWords.add(sortedWords.remove());
        }
        sortedWords.clear();
        /*
         * Sort top n words alphabetically
         */
        TreeMap<String, Integer> sortAlphabetize = new TreeMap<String, Integer>();
        for (Map.Entry<String, Integer> x : topNWords) {
            String key = x.getKey();
            int value = x.getValue();
            sortAlphabetize.put(key, value);
        }
        /*
         * Print HTML to the output stream.
         */
        printHeader(n, outFile, input);
        printBody(sortAlphabetize, outFile);
        printFooter(outFile);
        /*
         * Close i/o streams.
         */
        try {
            inFile.close();
            outFile.close();
            in.close();
            out.close();
        } catch (IOException e1) {
            System.err.println("Error closing files.");
            return;
        }

    }

}
