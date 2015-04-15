import components.naturalnumber.NaturalNumber;
import components.naturalnumber.NaturalNumberSecondary;

/**
 * {@code NaturalNumber} represented as a {@code String} with implementations of
 * primary methods.
 * 
 * @author Steven Miller, Chaz Gordish
 * 
 * @convention <pre>
 * {@code
 * [all characters of $this.rep are '0' through '9', with no leading '0']
 * }
 * </pre>
 * @correspondence <pre>
 * {@code 
 * this = [if $this.rep = "" then 0; else the decimal number whose ordinary
 *         depiction is $this.rep]
 * }
 * </pre>
 */
public class NaturalNumber3 extends NaturalNumberSecondary {

    /*
     * Private members --------------------------------------------------------
     */

    /**
     * Constant 10 (so it's not a "magic number" in the code).
     */
    private static final int TEN = 10;

    /**
     * Representation of {@code this}.
     */
    private String rep;

    /**
     * Creator of initial representation, an empty String.
     */
    private void createNewRep() {

        this.rep = "";

    }

    /*
     * Constructors -----------------------------------------------------------
     */

    /**
     * Default constructor.
     */
    public NaturalNumber3() {

        this.createNewRep();

    }

    /**
     * Constructor from {@code int}.
     * 
     * @param i
     *            {@code int} to initialize from
     */
    public NaturalNumber3(int i) {
        assert i >= 0 : "Violation of: i >= 0";

        if (i != 0) {
            // produces a String from int i
            this.rep = String.valueOf(i);
        } else {

            // needs to produce an empty String from int = 0
            this.createNewRep();
        }
    }

    /**
     * Constructor from {@code String}.
     * 
     * @param s
     *            {@code String} to initialize from
     */
    public NaturalNumber3(String s) {
        assert s != null : "Violation of: s is not null";
        assert s.matches("0|[1-9]\\d*") : ""
                + "Violation of: there exists n: NATURAL (s = TO_STRING(n))";

        if (s.equals("0")) {
            // makes the String "0" produce an empty String
            this.rep = "";
        } else {
            // otherwise, $this.rep = s is fine
            this.rep = s;
        }

    }

    /**
     * Constructor from {@code NaturalNumber}.
     * 
     * @param n
     *            {@code NaturalNumber} to initialize from
     */
    public NaturalNumber3(NaturalNumber n) {
        assert n != null : "Violation of: n is not null";

        if (!n.isZero()) {
            // if n is not 0, then $this.rep can be the String
            // representation of n
            this.rep = n.toString();
        } else {
            // if n is 0, $this.rep must be empty
            this.createNewRep();
        }
    }

    /*
     * Standard methods -------------------------------------------------------
     */

    @Override
    public final NaturalNumber newInstance() {
        try {
            return this.getClass().newInstance();
        } catch (ReflectiveOperationException e) {
            throw new AssertionError("Cannot construct object of type "
                    + this.getClass());
        }
    }

    @Override
    public final void clear() {
        this.createNewRep();
    }

    @Override
    public final void transferFrom(NaturalNumber source) {
        assert source != null : "Violation of: source is not null";
        assert source != this : "Violation of: source is not this";
        assert source instanceof NaturalNumber3 : ""
                + "Violation of: source is of dynamic type NaturalNumberExample";
        /*
         * This cast cannot fail since the assert above would have stopped
         * execution in that case.
         */
        NaturalNumber3 localSource = (NaturalNumber3) source;
        this.rep = localSource.rep;
        localSource.createNewRep();
    }

    /*
     * Kernel methods ---------------------------------------------------------
     */

    @Override
    public final void multiplyBy10(int k) {
        assert 0 <= k : "Violation of: 0 <= k";
        assert k < TEN : "Violation of: k < 10";

        // if both $this.rep and k are zero, then do nothing
        if (!(this.rep.equals("") && k == 0)) {

            // otherwise, concatenate the String representation of k
            // to the end of $this.rep
            this.rep = this.rep.concat(String.valueOf(k));
        }
    }

    @Override
    public final int divideBy10() {

        int result = 0;

        if (!this.rep.isEmpty()) {

            // take off last digit, keep as "lastDigit"
            String lastDigit = this.rep.substring(this.rep.length() - 1);
            this.rep = this.rep.substring(0, this.rep.length() - 1);

            // parse the String lastDigit into an integer
            // and set result
            result = Integer.parseInt(lastDigit);
        }
        // if $this.rep is empty, result remains zero.
        return result;
    }

    @Override
    public final boolean isZero() {

        // the empty String is the only valid representation of zero
        return this.rep.isEmpty();
    }
}
