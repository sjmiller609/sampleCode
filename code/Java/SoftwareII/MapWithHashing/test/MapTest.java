import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import components.map.Map;
import components.map.Map.Pair;

/**
 * JUnit test fixture for {@code Map<String, String>}'s constructor and kernel
 * methods.
 * 
 * @author Put your name here
 * 
 */
public abstract class MapTest {

    /**
     * Invokes the appropriate {@code Map} constructor for the implementation
     * under test and returns the result.
     * 
     * @return the new map
     * @ensures <pre>
     * {@code constructorTest = {}}
     * </pre>
     */
    protected abstract Map<String, String> constructorTest();

    /**
     * Invokes the appropriate {@code Map} constructor for the reference
     * implementation and returns the result.
     * 
     * @return the new map
     * @ensures <pre>
     * {@code constructorRef = {}}
     * </pre>
     */
    protected abstract Map<String, String> constructorRef();

    /**
     * 
     * Creates and returns a {@code Map<String, String>} of the implementation
     * under test type with the given entries.
     * 
     * @param args
     *            the (key, value) pairs for the map
     * @return the constructed map
     * @requires <pre>
     * {@code [args.length is even]  and  
     * [the 'key' entries in args are unique]}
     * </pre>
     * @ensures <pre>
     * {@code createFromArgsTest = [pairs in args]}
     * </pre>
     */
    private Map<String, String> createFromArgsTest(String... args) {
        assert args.length % 2 == 0 : "Violation of: args.length is even";
        Map<String, String> map = this.constructorTest();
        for (int i = 0; i < args.length; i += 2) {
            assert !map.hasKey(args[i]) : ""
                    + "Violation of: the 'key' entries in args are unique";
            map.add(args[i], args[i + 1]);
        }
        return map;
    }

    /**
     * 
     * Creates and returns a {@code Map<String, String>} of the reference
     * implementation type with the given entries.
     * 
     * @param args
     *            the (key, value) pairs for the map
     * @return the constructed map
     * @requires <pre>
     * {@code [args.length is even]  and  
     * [the 'key' entries in args are unique]}
     * </pre>
     * @ensures <pre>
     * {@code createFromArgsRef = [pairs in args]}
     * </pre>
     */
    private Map<String, String> createFromArgsRef(String... args) {
        assert args.length % 2 == 0 : "Violation of: args.length is even";
        Map<String, String> map = this.constructorRef();
        for (int i = 0; i < args.length; i += 2) {
            assert !map.hasKey(args[i]) : ""
                    + "Violation of: the 'key' entries in args are unique";
            map.add(args[i], args[i + 1]);
        }
        return map;
    }

    @Test
    public final void testDefaultConstructor() {
        /*
         * Set up variables and call method under test
         */
        Map<String, String> s = this.constructorTest();
        Map<String, String> sExpected = this.constructorRef();
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
        Map<String, String> s = this.createFromArgsTest();
        Map<String, String> sExpected = this.createFromArgsRef("key", "value");
        /*
         * Call method under test
         */
        s.add("key", "value");
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
        Map<String, String> s = this.createFromArgsTest("key", "value");
        Map<String, String> sExpected = this.createFromArgsRef("key", "value",
                "key1", "value1");
        /*
         * Call method under test
         */
        s.add("key1", "value1");
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
        Map<String, String> q = this.createFromArgsTest("key", "value", "key1",
                "value1");
        Map<String, String> qExpected = this.createFromArgsRef("key", "value",
                "key1", "value1", "key2", "value2");
        /*
         * Call method under test
         */
        q.add("key2", "value2");
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
        Map<String, String> q = this.createFromArgsTest("key", "value");
        Map<String, String> qExpected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        Pair<String, String> x = q.remove("key");

        /*
         * Assert that values of variables match expectations
         */

        assertEquals(qExpected, q);
        assertEquals("key", "value", x);
    }

    @Test
    public final void testRemoveLeavingNonEmptyOne() {
        /*
         * Set up variables
         */
        Map<String, String> q = this.createFromArgsTest("key", "value", "key1",
                "value1");
        Map<String, String> qExpected = this.createFromArgsRef("key", "value");
        /*
         * Call method under test
         */
        Pair<String, String> x = q.remove("key1");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals("key1", "value1", x);
    }

    @Test
    public final void testSizeEmpty() {
        /*
         * Set up variables
         */
        Map<String, String> q = this.createFromArgsTest();
        Map<String, String> qExpected = this.createFromArgsRef();
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
        Map<String, String> q = this.createFromArgsTest("key1", "value1");
        Map<String, String> qExpected = this
                .createFromArgsRef("key1", "value1");
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
        Map<String, String> q = this.createFromArgsTest("key1", "value1",
                "key", "value");
        Map<String, String> qExpected = this.createFromArgsRef("key1",
                "value1", "key", "value");
        /*
         * Call method under test
         */
        int i = q.size();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(2, i);
    }

    @Test
    public final void testHasKeyEmpty() {
        /*
         * Set up variables
         */
        Map<String, String> q = this.createFromArgsTest();
        Map<String, String> qExpected = this.createFromArgsTest();
        /*
         * Call method under test
         */
        boolean x = q.hasKey("blue");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(false, x);
    }

    @Test
    public final void testHasKeyOneTrue() {
        /*
         * Set up variables
         */
        Map<String, String> q = this.createFromArgsTest("key1", "value1");
        Map<String, String> qExpected = this.createFromArgsTest("key1",
                "value1");
        /*
         * Call method under test
         */
        boolean x = q.hasKey("key1");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(true, x);
    }

    @Test
    public final void testHasKeyOneFalse() {
        /*
         * Set up variables
         */
        Map<String, String> q = this.createFromArgsTest("key1", "value1");
        Map<String, String> qExpected = this.createFromArgsTest("key1",
                "value1");
        /*
         * Call method under test
         */
        boolean x = q.hasKey("key2");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(false, x);
    }

    @Test
    public final void testHasKeyMoreThanOneTrue() {
        /*
         * Set up variables
         */
        Map<String, String> q = this.createFromArgsTest("key1", "value1",
                "key2", "value2");
        Map<String, String> qExpected = this.createFromArgsRef("key1",
                "value1", "key2", "value2");
        /*
         * Call method under test
         */
        boolean x = q.hasKey("key2");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(x, true);
    }

    @Test
    public final void testContainsMoreThanOneFalse() {
        /*
         * Set up variables
         */
        Map<String, String> q = this.createFromArgsTest("key1", "value1",
                "key2", "value2");
        Map<String, String> qExpected = this.createFromArgsRef("key1",
                "value1", "key2", "value2");
        /*
         * Call method under test
         */
        boolean x = q.hasKey("key3");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(x, false);
    }

    @Test
    public final void testRemoveAnyOne() {
        /*
         * Set up variables
         */
        Map<String, String> q = this.createFromArgsTest("key1", "value1");
        Map<String, String> qExpected = this.createFromArgsRef();
        /*
         * Call method under test
         */
        Pair<String, String> x = q.removeAny();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals("key1", "value1", x);
    }

    @Test
    public final void testRemoveAnySeveral() {
        // this is the way Paolo taught us to test removeAny
        // in lab 1/24/14
        // modified for type Map 1/27/14
        /*
         * Set up variables
         */
        Map<String, String> s = this.createFromArgsTest("key1", "value1",
                "key2", "value2", "key3", "value3");
        Map<String, String> sExpect = this.createFromArgsRef("key1", "value1",
                "key2", "value2", "key3", "value3");

        /*
         * Call method under test
         */
        Pair<String, String> str = s.removeAny();
        /*
         * Assert that values of variables match expectations
         */
        assertTrue(sExpect.hasKey(str.key()));

        // match sExpect to s
        Pair<String, String> strExpect = sExpect.remove(str.key());
        assertEquals(strExpect.value(), str.value());
        assertEquals(sExpect, s);

    }

    @Test
    public final void testValue() {
        /*
         * Set up variables
         */
        Map<String, String> q = this.createFromArgsTest("key1", "value1",
                "key2", "value2");
        Map<String, String> qExpected = this.createFromArgsRef("key1",
                "value1", "key2", "value2");
        /*
         * Call method under test
         */
        String x = q.value("key2");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(x, "value2");
    }

    @Test
    public final void testValueOnePair() {
        /*
         * Set up variables
         */
        Map<String, String> q = this.createFromArgsTest("key1", "value1");
        Map<String, String> qExpected = this
                .createFromArgsRef("key1", "value1");
        /*
         * Call method under test
         */
        String x = q.value("key1");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(qExpected, q);
        assertEquals(x, "value1");
    }

}
