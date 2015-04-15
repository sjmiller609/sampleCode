import static org.junit.Assert.assertEquals;

import java.util.Comparator;

import org.junit.Test;

import components.sortingmachine.SortingMachine;

/**
 * JUnit test fixture for {@code SortingMachine<String>}'s constructor and
 * kernel methods.
 * 
 * @author Put your name here
 * 
 */
public abstract class SortingMachineTest {

    /**
     * Invokes the appropriate {@code SortingMachine} constructor for the
     * implementation under test and returns the result.
     * 
     * @param order
     *            the {@code Comparator} defining the order for {@code String}
     * @return the new {@code SortingMachine}
     * @requires <pre>
     * {@code IS_TOTAL_PREORDER([relation computed by order.compare method])}
     * </pre>
     * @ensures <pre>
     * {@code constructorTest = (true, order, {})}
     * </pre>
     */
    protected abstract SortingMachine<String> constructorTest(
            Comparator<String> order);

    /**
     * Invokes the appropriate {@code SortingMachine} constructor for the
     * reference implementation and returns the result.
     * 
     * @param order
     *            the {@code Comparator} defining the order for {@code String}
     * @return the new {@code SortingMachine}
     * @requires <pre>
     * {@code IS_TOTAL_PREORDER([relation computed by order.compare method])}
     * </pre>
     * @ensures <pre>
     * {@code constructorRef = (true, order, {})}
     * </pre>
     */
    protected abstract SortingMachine<String> constructorRef(
            Comparator<String> order);

    /**
     * 
     * Creates and returns a {@code SortingMachine<String>} of the
     * implementation under test type with the given entries and mode.
     * 
     * @param order
     *            the {@code Comparator} defining the order for {@code String}
     * @param insertionMode
     *            flag indicating the machine mode
     * @param args
     *            the entries for the {@code SortingMachine}
     * @return the constructed {@code SortingMachine}
     * @requires <pre>
     * {@code IS_TOTAL_PREORDER([relation computed by order.compare method])}
     * </pre>
     * @ensures <pre>
     * {@code createFromArgsTest = (insertionMode, order, [multiset of entries in args])}
     * </pre>
     */
    private SortingMachine<String> createFromArgsTest(Comparator<String> order,
            boolean insertionMode, String... args) {
        SortingMachine<String> sm = this.constructorTest(order);
        for (int i = 0; i < args.length; i++) {
            sm.add(args[i]);
        }
        if (!insertionMode) {
            sm.changeToExtractionMode();
        }
        return sm;
    }

    /**
     * 
     * Creates and returns a {@code SortingMachine<String>} of the reference
     * implementation type with the given entries and mode.
     * 
     * @param order
     *            the {@code Comparator} defining the order for {@code String}
     * @param insertionMode
     *            flag indicating the machine mode
     * @param args
     *            the entries for the {@code SortingMachine}
     * @return the constructed {@code SortingMachine}
     * @requires <pre>
     * {@code IS_TOTAL_PREORDER([relation computed by order.compare method])}
     * </pre>
     * @ensures <pre>
     * {@code createFromArgsRef = (insertionMode, order, [multiset of entries in args])}
     * </pre>
     */
    private SortingMachine<String> createFromArgsRef(Comparator<String> order,
            boolean insertionMode, String... args) {
        SortingMachine<String> sm = this.constructorRef(order);
        for (int i = 0; i < args.length; i++) {
            sm.add(args[i]);
        }
        if (!insertionMode) {
            sm.changeToExtractionMode();
        }
        return sm;
    }

    /**
     * Comparator<String> implementation to be used in all test cases. Compare
     * {@code String}s in lexicographic order.
     */
    private static class StringLT implements Comparator<String> {

        @Override
        public int compare(String s1, String s2) {
            return s1.compareToIgnoreCase(s2);
        }

    }

    /**
     * Comparator instance to be used in all test cases.
     */
    private static final StringLT ORDER = new StringLT();

    /*
     * Sample test cases.
     */

    @Test
    public final void testConstructor() {
        SortingMachine<String> m = this.constructorTest(ORDER);
        SortingMachine<String> mExpected = this.constructorRef(ORDER);
        assertEquals(mExpected, m);
    }

    /*
     * Test cases for kernel methods
     */

    @Test
    public final void testAddEmpty() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true);
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, true,
                "green");
        m.add("green");
        assertEquals(mExpected, m);
    }

    @Test
    public final void testAddToOne() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true, "red");
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, true,
                "red", "green");
        m.add("green");
        assertEquals(mExpected, m);
    }

    @Test
    public final void testAddToOneDuplicate() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true, "red");
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, true,
                "red", "red");
        m.add("red");
        assertEquals(mExpected, m);
    }

    @Test
    public final void testAddToMoreThanOne() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true, "red",
                "blue", "green", "a", "b", "c", "d", "e", "f", "g", "h", "i",
                "j", "k");
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, true,
                "green", "red", "blue", "green", "a", "b", "c", "d", "e", "f",
                "g", "h", "i", "j", "k");
        m.add("green");
        assertEquals(mExpected, m);

    }

    @Test
    public final void testAddWithDuplicates() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true, "red",
                "red", "red", "red", "red", "red", "red", "red");
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, true,
                "red", "red", "red", "red", "red", "red", "red", "red", "red");
        m.add("red");
        assertEquals(mExpected, m);

    }

    @Test
    public final void testAddWithSomeDuplicates() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true, "red",
                "green", "red", "green", "red", "green", "red", "green", "red",
                "green", "red", "green", "red", "green");
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, true,
                "red", "green", "red", "green", "red", "green", "red", "green",
                "red", "green", "red", "green", "red", "green", "red");
        m.add("red");
        assertEquals(mExpected, m);

    }

    @Test
    public final void testRemoveFirstLeavingEmpty() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false, "red");
        SortingMachine<String> qExpected = this
                .createFromArgsTest(ORDER, false);
        String x = q.removeFirst();
        assertEquals(qExpected, q);
        assertEquals("red", x);
    }

    @Test
    public final void testRemoveFirstLeavingOne() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false, "red",
                "blue");
        SortingMachine<String> qExpected = this.createFromArgsTest(ORDER,
                false, "red");
        String x = q.removeFirst();
        assertEquals(qExpected, q);
        assertEquals("blue", x);
    }

    @Test
    public final void testRemoveFirstLeavingOneDuplicate() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false, "red",
                "red");
        SortingMachine<String> qExpected = this.createFromArgsTest(ORDER,
                false, "red");
        String x = q.removeFirst();
        assertEquals(qExpected, q);
        assertEquals("red", x);
    }

    @Test
    public final void testRemoveFirstLeavingMany() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false, "red",
                "blue", "yellow", "green", "a", "b", "c", "d", "e", "f", "g",
                "h", "i", "j", "k");
        SortingMachine<String> qExpected = this.createFromArgsTest(ORDER,
                false, "red", "blue", "yellow", "green", "b", "c", "d", "e",
                "f", "g", "h", "i", "j", "k");
        String x = q.removeFirst();
        assertEquals(qExpected, q);
        assertEquals("a", x);
    }

    @Test
    public final void testRemoveFirstWithDuplicates() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false, "red",
                "red", "red", "red", "red", "red", "red", "red");
        SortingMachine<String> qExpected = this.createFromArgsTest(ORDER,
                false, "red", "red", "red", "red", "red", "red", "red");
        String x = q.removeFirst();
        assertEquals(qExpected, q);
        assertEquals("red", x);
    }

    @Test
    public final void testRemoveFirstWithSomeDuplicates() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false, "red",
                "green", "red", "green", "red", "green", "red", "green", "red",
                "green", "red", "green", "red", "green", "red", "green");
        SortingMachine<String> qExpected = this.createFromArgsTest(ORDER,
                false, "red", "green", "red", "green", "red", "green", "red",
                "green", "red", "red", "green", "red", "green", "red", "green");
        String x = q.removeFirst();
        assertEquals(qExpected, q);
        assertEquals("green", x);
    }

    @Test
    public final void testSizeEmptyExtractionMode() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false);
        SortingMachine<String> qExpected = this
                .createFromArgsTest(ORDER, false);
        int i = q.size();
        assertEquals(qExpected, q);
        assertEquals(0, i);
    }

    @Test
    public final void testSizeEmptyInsertionMode() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, true);
        SortingMachine<String> qExpected = this.createFromArgsTest(ORDER, true);
        int i = q.size();
        assertEquals(qExpected, q);
        assertEquals(0, i);
    }

    @Test
    public final void testSizeOneExtractionMode() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false, "red");
        SortingMachine<String> qExpected = this.createFromArgsTest(ORDER,
                false, "red");
        int i = q.size();
        assertEquals(qExpected, q);
        assertEquals(1, i);
    }

    @Test
    public final void testSizeOneInsertionMode() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, true, "red");
        SortingMachine<String> qExpected = this.createFromArgsTest(ORDER, true,
                "red");
        int i = q.size();
        assertEquals(qExpected, q);
        assertEquals(1, i);
    }

    @Test
    public final void testSizeManyExtractionMode() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false, "red",
                "blue", "yellow", "green", "a", "b", "c", "d", "e", "f", "g",
                "h", "i", "j", "k");
        SortingMachine<String> qExpected = this.createFromArgsRef(ORDER, false,
                "red", "blue", "green", "a", "b", "c", "d", "e", "f", "g", "h",
                "i", "j", "k", "yellow");

        int i = q.size();
        assertEquals(qExpected, q);
        assertEquals(15, i);
    }

    @Test
    public final void testSizeManyExtractionModeSomeDuplicates() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false, "red",
                "green", "red", "green", "red", "green", "red", "green", "red",
                "green", "red", "green", "red", "green", "red", "green");
        SortingMachine<String> qExpected = this.createFromArgsTest(ORDER,
                false, "red", "green", "red", "green", "red", "green", "red",
                "green", "red", "green", "red", "green", "red", "green", "red",
                "green");

        int i = q.size();
        assertEquals(qExpected, q);
        assertEquals(16, i);
    }

    @Test
    public final void testSizeManyExtractionModeDuplicates() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, false, "red",
                "red", "red", "red", "red", "red", "red", "red");
        SortingMachine<String> qExpected = this.createFromArgsRef(ORDER, false,
                "red", "red", "red", "red", "red", "red", "red", "red");

        int i = q.size();
        assertEquals(qExpected, q);
        assertEquals(8, i);
    }

    @Test
    public final void testSizeManyInsertionMode() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, true, "red",
                "blue", "yellow", "green", "a", "b", "c", "d", "e", "f", "g",
                "h", "i", "j", "k");
        SortingMachine<String> qExpected = this.createFromArgsRef(ORDER, true,
                "red", "blue", "green", "a", "b", "c", "d", "e", "f", "g", "h",
                "i", "j", "k", "yellow");

        int i = q.size();
        assertEquals(qExpected, q);
        assertEquals(15, i);
    }

    @Test
    public final void testSizeManyInsertionModeDuplicates() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, true, "red",
                "red", "red", "red", "red", "red", "red", "red");
        SortingMachine<String> qExpected = this.createFromArgsRef(ORDER, true,
                "red", "red", "red", "red", "red", "red", "red", "red");

        int i = q.size();
        assertEquals(qExpected, q);
        assertEquals(8, i);
    }

    @Test
    public final void testSizeManyInsertionModeSomeDuplicates() {

        SortingMachine<String> q = this.createFromArgsTest(ORDER, true, "red",
                "green", "red", "green", "red", "green", "red", "green", "red",
                "green", "red", "green", "red", "green", "red", "green");
        SortingMachine<String> qExpected = this.createFromArgsTest(ORDER, true,
                "red", "green", "red", "green", "red", "green", "red", "green",
                "red", "green", "red", "green", "red", "green", "red", "green");

        int i = q.size();
        assertEquals(qExpected, q);
        assertEquals(16, i);
    }

    @Test
    public final void testChangeToExtractionModeEmpty() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true);
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, false);
        m.changeToExtractionMode();
        assertEquals(mExpected, m);

    }

    @Test
    public final void testChangeToExtractionModeOne() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true, "red");
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, false,
                "red");
        m.changeToExtractionMode();
        assertEquals(mExpected, m);

    }

    @Test
    public final void testChangeToExtractionModeMany() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true, "red",
                "blue", "green", "a", "b", "c", "d", "e", "f", "g", "h", "i",
                "j", "k");
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, false,
                "red", "blue", "green", "a", "b", "c", "d", "e", "f", "g", "h",
                "i", "j", "k");
        m.changeToExtractionMode();
        assertEquals(mExpected, m);

    }

    @Test
    public final void testChangeToExtractionModeDuplicates() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true, "red",
                "red", "red", "red", "red", "red", "red", "red");
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, false,
                "red", "red", "red", "red", "red", "red", "red", "red");
        m.changeToExtractionMode();
        assertEquals(mExpected, m);

    }

    @Test
    public final void testChangeToExtractionModeSomeDuplicates() {
        SortingMachine<String> m = this.createFromArgsTest(ORDER, true, "red",
                "green", "red", "green", "red", "green", "red", "green", "red",
                "green", "red", "green", "red", "green", "red", "green");
        SortingMachine<String> mExpected = this.createFromArgsRef(ORDER, false,
                "red", "green", "red", "green", "red", "green", "red", "green",
                "red", "green", "red", "green", "red", "green", "red", "green");
        m.changeToExtractionMode();
        assertEquals(mExpected, m);

    }
}
