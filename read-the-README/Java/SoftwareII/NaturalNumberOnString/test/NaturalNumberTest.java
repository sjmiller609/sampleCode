import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import components.naturalnumber.NaturalNumber;
import components.naturalnumber.NaturalNumber1L;

/**
 * JUnit test fixture for {@code NaturalNumber}'s constructors and kernel
 * methods.
 * 
 * @author Steven Miller, Chaz Gordish
 * 
 */
public abstract class NaturalNumberTest {

    /**
     * Invokes the appropriate {@code NaturalNumber} constructor for the
     * implementation under test and returns the result.
     * 
     * @return the new number
     * @ensures <pre>
     * {@code constructorTest = 0}
     * </pre>
     */
    protected abstract NaturalNumber constructorTest();

    /**
     * Invokes the appropriate {@code NaturalNumber} constructor for the
     * implementation under test and returns the result.
     * 
     * @param i
     *            {@code int} to initialize from
     * @return the new number
     * @requires <pre>
     * {@code i >= 0}
     * </pre>
     * @ensures <pre>
     * {@code constructorTest = i}
     * </pre>
     */
    protected abstract NaturalNumber constructorTest(int i);

    /**
     * Invokes the appropriate {@code NaturalNumber} constructor for the
     * implementation under test and returns the result.
     * 
     * @param s
     *            {@code String} to initialize from
     * @return the new number
     * @requires <pre>
     * {@code there exists n: NATURAL (s = TO_STRING(n))}
     * </pre>
     * @ensures <pre>
     * {@code s = TO_STRING(constructorTest)}
     * </pre>
     */
    protected abstract NaturalNumber constructorTest(String s);

    /**
     * Invokes the appropriate {@code NaturalNumber} constructor for the
     * implementation under test and returns the result.
     * 
     * @param n
     *            {@code NaturalNumber} to initialize from
     * @return the new number
     * @ensures <pre>
     * {@code constructorTest = n}
     * </pre>
     */
    protected abstract NaturalNumber constructorTest(NaturalNumber n);

    /**
     * Invokes the appropriate {@code NaturalNumber} constructor for the
     * reference implementation and returns the result.
     * 
     * @return the new number
     * @ensures <pre>
     * {@code constructorRef = 0}
     * </pre>
     */
    protected abstract NaturalNumber constructorRef();

    /**
     * Invokes the appropriate {@code NaturalNumber} constructor for the
     * reference implementation and returns the result.
     * 
     * @param i
     *            {@code int} to initialize from
     * @return the new number
     * @requires <pre>
     * {@code i >= 0}
     * </pre>
     * @ensures <pre>
     * {@code constructorRef = i}
     * </pre>
     */
    protected abstract NaturalNumber constructorRef(int i);

    /**
     * Invokes the appropriate {@code NaturalNumber} constructor for the
     * reference implementation and returns the result.
     * 
     * @param s
     *            {@code String} to initialize from
     * @return the new number
     * @requires <pre>
     * {@code there exists n: NATURAL (s = TO_STRING(n))}
     * </pre>
     * @ensures <pre>
     * {@code s = TO_STRING(constructorRef)}
     * </pre>
     */
    protected abstract NaturalNumber constructorRef(String s);

    /**
     * Invokes the appropriate {@code NaturalNumber} constructor for the
     * reference implementation and returns the result.
     * 
     * @param n
     *            {@code NaturalNumber} to initialize from
     * @return the new number
     * @ensures <pre>
     * {@code constructorRef = n}
     * </pre>
     */
    protected abstract NaturalNumber constructorRef(NaturalNumber n);

    @Test
    public final void testDefaultConstructor() {
        /*
         * Set up variables and call method under test
         */
        NaturalNumber nat = this.constructorTest();
        NaturalNumber natExpected = this.constructorRef();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    @Test
    public final void testIntConstructor() {
        /*
         * Set up variables and call method under test
         */
        NaturalNumber nat = this.constructorTest(7);
        NaturalNumber natExpected = this.constructorRef(7);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    @Test
    public final void testIntConstructorZero() {
        /*
         * Set up variables and call method under test
         */
        NaturalNumber nat = this.constructorTest(0);
        NaturalNumber natExpected = this.constructorRef(0);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    @Test
    public final void testStringConstructor() {
        /*
         * Set up variables and call method under test
         */
        NaturalNumber nat = this.constructorTest("7");
        NaturalNumber natExpected = this.constructorRef("7");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    @Test
    public final void testStringConstructorZero() {
        /*
         * Set up variables and call method under test
         */
        NaturalNumber nat = this.constructorTest("0");
        NaturalNumber natExpected = this.constructorRef("0");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    @Test
    public final void testNaturalNumberConstructor() {
        /*
         * Set up variables and call method under test
         */
        NaturalNumber nat = this.constructorTest(new NaturalNumber1L(7));
        NaturalNumber natExpected = this.constructorRef(new NaturalNumber1L(7));
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    @Test
    public final void testNaturalNumberConstructorZero() {
        /*
         * Set up variables and call method under test
         */
        NaturalNumber nat = this.constructorTest(new NaturalNumber1L(0));
        NaturalNumber natExpected = this.constructorRef(new NaturalNumber1L(0));
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    /*
     * Kernel Methods
     */
    @Test
    public final void testMultiplyBy10() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(13);
        NaturalNumber natExpected = this.constructorRef(137);
        /*
         * Call method under test
         */
        nat.multiplyBy10(7);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    @Test
    public final void testMultiplyBy10Zero() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(0);
        NaturalNumber natExpected = this.constructorRef(0);
        /*
         * Call method under test
         */
        nat.multiplyBy10(0);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    @Test
    public final void testMultiplyBy10ZeroAddNumber() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(0);
        NaturalNumber natExpected = this.constructorRef(6);
        /*
         * Call method under test
         */
        nat.multiplyBy10(6);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    @Test
    public final void testMultiplyBy10AddZero() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(4);
        NaturalNumber natExpected = this.constructorRef(40);
        /*
         * Call method under test
         */
        nat.multiplyBy10(0);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
    }

    @Test
    public final void testDivideBy10() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(147);
        NaturalNumber natExpected = this.constructorRef(14);
        /*
         * Call method under test
         */
        int temp = nat.divideBy10();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
        assertEquals(7, temp);
    }

    @Test
    public final void testDivideBy10Zero() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(0);
        NaturalNumber natExpected = this.constructorRef(0);
        /*
         * Call method under test
         */
        int temp = nat.divideBy10();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
        assertEquals(0, temp);
    }

    @Test
    public final void testDivideBy10LeaveAsZero() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(7);
        NaturalNumber natExpected = this.constructorRef(0);
        /*
         * Call method under test
         */
        int temp = nat.divideBy10();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
        assertEquals(7, temp);
    }

    @Test
    public final void testDivideBy10ReturnZero() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(70);
        NaturalNumber natExpected = this.constructorRef(7);
        /*
         * Call method under test
         */
        int temp = nat.divideBy10();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(natExpected, nat);
        assertEquals(0, temp);
    }

    @Test
    public final void testIsZeroFalse() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(70);
        /*
         * Call method under test
         */
        /*
         * Assert that values of variables match expectations
         */
        assertFalse(nat.isZero());
    }

    @Test
    public final void testIsZeroTrue() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(0);
        /*
         * Call method under test
         */
        /*
         * Assert that values of variables match expectations
         */
        assertTrue(nat.isZero());
    }

    @Test
    public final void testIsZeroTrueDefault() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest();
        /*
         * Call method under test
         */
        /*
         * Assert that values of variables match expectations
         */
        assertTrue(nat.isZero());
    }

    @Test
    public final void testIsZeroFalseLarge() {
        /*
         * Set up variables
         */
        NaturalNumber nat = this.constructorTest(283647852);
        /*
         * Call method under test
         */
        /*
         * Assert that values of variables match expectations
         */
        assertFalse(nat.isZero());
    }
}
