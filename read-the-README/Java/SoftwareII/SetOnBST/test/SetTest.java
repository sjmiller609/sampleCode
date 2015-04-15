import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import components.set.Set;

/**
 * JUnit test fixture for {@code Set<String>}'s constructor and kernel methods.
 * 
 * @author Put your name here
 * 
 */
public abstract class SetTest {

    /**
     * Invokes the appropriate {@code Set} constructor for the implementation
     * under test and returns the result.
     * 
     * @return the new set
     * @ensures <pre>
     * {@code constructorTest = {}}
     * </pre>
     */
    protected abstract Set<String> constructorTest();

    /**
     * Invokes the appropriate {@code Set} constructor for the reference
     * implementation and returns the result.
     * 
     * @return the new set
     * @ensures <pre>
     * {@code constructorRef = {}}
     * </pre>
     */
    protected abstract Set<String> constructorRef();

    /**
     * Creates and returns a {@code Set<String>} of the implementation under
     * test type with the given entries.
     * 
     * @param args
     *            the entries for the set
     * @return the constructed set
     * @requires <pre>
     * {@code [every entry in args is unique]}
     * </pre>
     * @ensures <pre>
     * {@code createFromArgsTest = [entries in args]}
     * </pre>
     */
    private Set<String> createFromArgsTest(String... args) {
        Set<String> set = this.constructorTest();
        for (String s : args) {
            assert !set.contains(s) : "Violation of: every entry in args is unique";
            set.add(s);
        }
        return set;
    }

    /**
     * Creates and returns a {@code Set<String>} of the reference implementation
     * type with the given entries.
     * 
     * @param args
     *            the entries for the set
     * @return the constructed set
     * @requires <pre>
     * {@code [every entry in args is unique]}
     * </pre>
     * @ensures <pre>
     * {@code createFromArgsRef = [entries in args]}
     * </pre>
     */
    private Set<String> createFromArgsRef(String... args) {
        Set<String> set = this.constructorRef();
        for (String s : args) {
            assert !set.contains(s) : "Violation of: every entry in args is unique";
            set.add(s);
        }
        return set;
    }

    @Test
    public final void testDefaultConstructor() {
        /*
         * Set up variables and call method under test
         */
        Set<String> s = this.constructorTest();
        Set<String> sExpected = this.constructorRef();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(sExpected, s);
    }

    /*
     * Test cases for kernel methods
     */

    @Test
    public final void testAddEmpty() {
        /*
         * Set up variables
         */
        Set<String> s = this.createFromArgsTest();
        Set<String> sExpected = this.createFromArgsRef("red");
        /*
         * Call method under test
         */
        s.add("red");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(sExpected, s);
    }

    @Test
    public final void testAddNonEmptyOne() {
        /*
         * Set up variables
         */
        Set<String> s = this.createFromArgsTest("red");
        Set<String> sExpected = this.createFromArgsRef("blue", "red");
        /*
         * Call method under test
         */
        s.add("blue");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(sExpected, s);
    }

    @Test
    public final void testAddNonEmptyOneAtEnd() {
        /*
         * Set up variables
         */
        Set<String> s = this.createFromArgsTest("red");
        Set<String> sExpected = this.createFromArgsRef("red", "blue");
        /*
         * Call method under test
         */
        s.add("blue");
        /*
         * Assert that values of variables match expectations
         */

        assertEquals(sExpected, s);
    }

    @Test
    public final void testAddNonEmptyMoreThanOne() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red", "blue", "green");
        Set<String> qExpected = this.createFromArgsRef("yellow", "red", "blue",
                "green");
        /*
         * Call method under test
         */
        q.add("yellow");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
    }

    @Test
    public final void testAddNonEmptyMoreThanOneInMiddle() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red", "blue", "green");
        Set<String> qExpected = this.createFromArgsRef("red", "yellow", "blue",
                "green");
        /*
         * Call method under test
         */
        q.add("yellow");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
    }

    @Test
    public final void testAddNonEmptyMoreThanOneAtEnd() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red", "blue", "green");
        Set<String> qExpected = this.createFromArgsRef("red", "blue", "green",
                "yellow");
        /*
         * Call method under test
         */
        q.add("yellow");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
    }

    @Test
    public final void testRemoveLeavingEmpty() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red");
        Set<String> qExpected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        String x = q.remove("red");

        /*
         * Assert that values of variables match expectations
         */

        assertEquals(qExpected, q);
        assertEquals("red", x);
    }

    @Test
    public final void testRemoveLeavingNonEmptyOne() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red", "blue");
        Set<String> qExpected = this.createFromArgsRef("blue");
        /*
         * Call method under test
         */
        String x = q.remove("red");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals("red", x);
    }

    @Test
    public final void testRemoveLeavingNonEmptyOneFromEnd() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red", "blue");
        Set<String> qExpected = this.createFromArgsRef("red");
        /*
         * Call method under test
         */
        String x = q.remove("blue");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals("blue", x);
    }

    @Test
    public final void testRemoveLeavingNonEmptyMoreThanOneTakenFromMiddle() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red", "green", "blue");
        Set<String> qExpected = this.createFromArgsRef("red", "blue");
        /*
         * Call method under test
         */
        String x = q.remove("green");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals("green", x);
    }

    @Test
    public final void testSizeEmpty() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest();
        Set<String> qExpected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        int i = q.size();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(0, i);
    }

    @Test
    public final void testSizeNonEmptyOne() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red");
        Set<String> qExpected = this.createFromArgsRef("red");
        /*
         * Call method under test
         */
        int i = q.size();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(1, i);
    }

    @Test
    public final void testSizeNonEmptyMoreThanOne() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red", "green", "blue");
        Set<String> qExpected = this.createFromArgsRef("red", "green", "blue");
        /*
         * Call method under test
         */
        int i = q.size();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(3, i);
    }

    @Test
    public final void testContainsEmpty() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest();
        Set<String> qExpected = this.createFromArgsTest();
        /*
         * Call method under test
         */
        boolean x = q.contains("blue");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertFalse(x);
    }

    @Test
    public final void testContainsOneTrue() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("blue");
        Set<String> qExpected = this.createFromArgsTest("blue");
        /*
         * Call method under test
         */
        boolean x = q.contains("blue");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertTrue(x);
    }

    @Test
    public final void testContainsOneFalse() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("blue");
        Set<String> qExpected = this.createFromArgsTest("blue");
        /*
         * Call method under test
         */
        boolean x = q.contains("red");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertFalse(x);
    }

    @Test
    public final void testContainsMoreThanOneTrue() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red", "green", "blue");
        Set<String> qExpected = this.createFromArgsRef("red", "green", "blue");
        /*
         * Call method under test
         */
        boolean x = q.contains("green");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertTrue(x);
    }

    @Test
    public final void testContainsMoreThanOneFalse() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red", "green", "blue");
        Set<String> qExpected = this.createFromArgsRef("red", "green", "blue");
        /*
         * Call method under test
         */
        boolean x = q.contains("yellow");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertFalse(x);
    }

    @Test
    public final void testRemoveAnyOne() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red");
        Set<String> qExpected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        String x = q.removeAny();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals("red", x);
    }

    @Test
    public final void testRemoveAnySeveral() {
        // @CHAZ: this is the way Paolo taught us to test removeAny
        // in lab 1/24/14
        /*
         * Set up variables
         */
        Set<String> s = this.createFromArgsTest("red", "green", "yellow");
        Set<String> sExpect = this.createFromArgsRef("red", "yellow", "green");

        /*
         * Call method under test
         */
        String str = s.removeAny();
        /*
         * Assert that values of variables match expectations
         */
        assertTrue(sExpect.contains(str));

        // match sExpect to s
        sExpect.remove(str);

        assertEquals(sExpect, s);

    }

    /*
     * Test cases for Standard methods
     */

    @Test
    public final void testNewInstanceEmpty() {
        /*
         * Set up variables
         */
        Set<String> q1 = this.createFromArgsTest();
        Set<String> q1Expected = this.createFromArgsRef();
        Set<String> q2Expected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        Set<String> q2 = q1.newInstance();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(q1Expected, q1);
        assertEquals(q2Expected, q2);
    }

    @Test
    public final void testNewInstanceNonEmptyOne() {
        /*
         * Set up variables
         */
        Set<String> q1 = this.createFromArgsTest("red");
        Set<String> q1Expected = this.createFromArgsRef("red");
        Set<String> q2Expected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        Set<String> q2 = q1.newInstance();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(q1Expected, q1);
        assertEquals(q2Expected, q2);
    }

    @Test
    public final void testNewInstanceNonEmptyMoreThanOne() {
        /*
         * Set up variables
         */
        Set<String> q1 = this.createFromArgsTest("red", "green", "blue");
        Set<String> q1Expected = this.createFromArgsRef("red", "green", "blue");
        Set<String> q2Expected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        Set<String> q2 = q1.newInstance();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(q1Expected, q1);
        assertEquals(q2Expected, q2);
    }

    @Test
    public final void testClearEmpty() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest();
        Set<String> qExpected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        q.clear();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
    }

    @Test
    public final void testClearNonEmptyOne() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red");
        Set<String> qExpected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        q.clear();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
    }

    @Test
    public final void testClearNonEmptyMoreThanOne() {
        /*
         * Set up variables
         */
        Set<String> q = this.createFromArgsTest("red", "green", "blue");
        Set<String> qExpected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        q.clear();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
    }

    @Test
    public final void testTransferFromEmptyEmpty() {
        /*
         * Set up variables
         */
        Set<String> q1 = this.createFromArgsTest();
        Set<String> q1Expected = this.createFromArgsRef();
        Set<String> q2 = this.createFromArgsTest();
        Set<String> q2Expected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        q1.transferFrom(q2);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(q1Expected, q1);
        assertEquals(q2Expected, q2);
    }

    @Test
    public final void testTransferFromEmptyNonEmpty() {
        /*
         * Set up variables
         */
        Set<String> q1 = this.createFromArgsTest();
        Set<String> q1Expected = this.createFromArgsRef("red", "green", "blue");
        Set<String> q2 = this.createFromArgsTest("red", "green", "blue");
        Set<String> q2Expected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        q1.transferFrom(q2);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(q1Expected, q1);
        assertEquals(q2Expected, q2);
    }

    @Test
    public final void testTransferFromNonEmptyEmpty() {
        /*
         * Set up variables
         */
        Set<String> q1 = this.createFromArgsTest("red", "green", "blue");
        Set<String> q1Expected = this.createFromArgsRef();
        Set<String> q2 = this.createFromArgsTest();
        Set<String> q2Expected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        q1.transferFrom(q2);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(q1Expected, q1);
        assertEquals(q2Expected, q2);
    }

    @Test
    public final void testTransferFromNonEmptyNonEmpty() {
        /*
         * Set up variables
         */
        Set<String> q1 = this.createFromArgsTest("red", "green", "blue");
        Set<String> q1Expected = this.createFromArgsRef("yellow", "orange");
        Set<String> q2 = this.createFromArgsTest("yellow", "orange");
        Set<String> q2Expected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        q1.transferFrom(q2);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(q1Expected, q1);
        assertEquals(q2Expected, q2);
    }

}
