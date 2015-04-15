import components.sequence.Sequence;
import components.sequence.Sequence3;
import components.statement.Statement;
import components.statement.StatementSecondary;
import components.tree.Tree;
import components.tree.Tree1;
import components.utilities.Tokenizer;

/**
 * {@code Statement} represented as a {@code Tree<StatementLabel>} with
 * implementations of primary methods.
 * 
 * @convention <pre>
 * {@code [$this.rep is a valid representation of a Statement]}
 * </pre>
 * @correspondence <pre>
 * {@code this = $this.rep}
 * </pre>
 */
public class Statement2 extends StatementSecondary {

    /*
     * Private members --------------------------------------------------------
     */

    /**
     * Label class for the tree representation.
     */
    private static final class StatementLabel {

        /**
         * Statement kind.
         */
        private Kind kind;

        /**
         * IF/IF_ELSE/WHILE statement condition.
         */
        private Condition condition;

        /**
         * CALL instruction name.
         */
        private String instruction;

        /**
         * Constructor for BLOCK.
         * 
         * @param k
         *            the kind of statement
         */
        private StatementLabel(Kind k) {
            assert k == Kind.BLOCK : "Violation of: k = BLOCK";
            this.kind = k;
        }

        /**
         * Constructor for IF, IF_ELSE, WHILE.
         * 
         * @param k
         *            the kind of statement
         * @param c
         *            the statement condition
         */
        private StatementLabel(Kind k, Condition c) {
            assert k == Kind.IF || k == Kind.IF_ELSE || k == Kind.WHILE : ""
                    + "Violation of: k = IF or k = IF_ELSE or k = WHILE";
            this.kind = k;
            this.condition = c;
        }

        /**
         * Constructor for CALL.
         * 
         * @param k
         *            the kind of statement
         * @param i
         *            the instruction name
         */
        private StatementLabel(Kind k, String i) {
            assert k == Kind.CALL : "Violation of: k = CALL";
            assert i != null : "Violation of: i is not null";
            assert Tokenizer.isIdentifier(i) : "Violation of: i is an IDENTIFIER";
            this.kind = k;
            this.instruction = i;
        }
    }

    /**
     * The tree representation field.
     */
    private Tree<StatementLabel> rep;

    /**
     * Creator of initial representation.
     */
    private void createNewRep() {

        StatementLabel blockRoot = new StatementLabel(Kind.BLOCK);
        Sequence<Tree<StatementLabel>> seq = new Sequence3<Tree<StatementLabel>>();
        this.rep = new Tree1<>();
        this.rep.assemble(blockRoot, seq);

    }

    /*
     * Constructors -----------------------------------------------------------
     */

    /**
     * Default constructor.
     */
    public Statement2() {
        this.createNewRep();
    }

    /*
     * Standard methods -------------------------------------------------------
     */

    @Override
    public final Statement2 newInstance() {
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
    public final void transferFrom(Statement source) {
        assert source != null : "Violation of: source is not null";
        assert source != this : "Violation of: source is not this";
        assert source instanceof Statement2 : ""
                + "Violation of: source is of dynamic type Statement2";
        /*
         * This cast cannot fail since the assert above would have stopped
         * execution in that case: source must be of dynamic type Statement2.
         */
        Statement2 localSource = (Statement2) source;
        this.rep = localSource.rep;
        localSource.createNewRep();
    }

    /*
     * Kernel methods ---------------------------------------------------------
     */

    @Override
    public final Kind kind() {

        Sequence<Tree<StatementLabel>> children = this.rep.newSequenceOfTree();
        StatementLabel root = this.rep.disassemble(children);
        Kind result = root.kind;
        this.rep.assemble(root, children);
        return result;

    }

    @Override
    public final void addToBlock(int pos, Statement s) {
        assert s != null : "Violation of: s is not null";
        assert s instanceof Statement2 : "Violation of: s is a Statement2";
        assert this.kind() == Kind.BLOCK : ""
                + "Violation of: [this is a BLOCK statement]";
        assert 0 <= pos : "Violation of: 0 <= pos";
        assert pos <= this.lengthOfBlock() : ""
                + "Violation of: pos <= [length of this BLOCK]";
        assert s.kind() != Kind.BLOCK : "Violation of: [s is not a BLOCK statement]";

        // not sure
        Statement2 localS = (Statement2) s;
        Sequence<Tree<StatementLabel>> children = this.rep.newSequenceOfTree();
        StatementLabel label = this.rep.disassemble(children);
        children.add(pos, localS.rep);
        this.rep.assemble(label, children);
        s.clear();

    }

    @Override
    public final Statement removeFromBlock(int pos) {
        assert 0 <= pos : "Violation of: 0 <= pos";
        assert pos < this.lengthOfBlock() : ""
                + "Violation of: pos < [length of this BLOCK]";
        assert this.kind() == Kind.BLOCK : ""
                + "Violation of: [this is a BLOCK statement]";
        /*
         * The following call to Statement newInstance method is a violation of
         * the kernel purity rule. However, there is no way to avoid it and it
         * is safe because the convention clearly holds at this point in the
         * code.
         */

        Statement2 s = this.newInstance();
        Sequence<Tree<StatementLabel>> children = this.rep.newSequenceOfTree();
        StatementLabel root = this.rep.disassemble(children);
        Tree<StatementLabel> resultTree = children.remove(pos);
        s.rep = resultTree;
        this.rep.assemble(root, children);

        return s;
    }

    @Override
    public final int lengthOfBlock() {
        assert this.kind() == Kind.BLOCK : ""
                + "Violation of: [this is a BLOCK statement]";

        return this.rep.numberOfSubtrees();

    }

    @Override
    public final void assembleIf(Condition c, Statement s) {
        assert c != null : "Violation of: c is not null";
        assert s != null : "Violation of: s is not null";
        assert s instanceof Statement2 : "Violation of: s is a Statement2";
        assert s.kind() == Kind.BLOCK : ""
                + "Violation of: [s is a BLOCK statement]";

        Statement2 localS = (Statement2) s;
        StatementLabel label = new StatementLabel(Kind.IF, c);
        Sequence<Tree<StatementLabel>> children = this.rep.newSequenceOfTree();
        children.add(0, localS.rep);
        this.rep.assemble(label, children);
        localS.createNewRep(); // clears s
    }

    @Override
    public final Condition disassembleIf(Statement s) {
        assert s != null : "Violation of: s is not null";
        assert s instanceof Statement2 : "Violation of: s is a Statement2";
        assert this.kind() == Kind.IF : "Violation of: [s is an IF statement]";

        Statement2 localS = (Statement2) s;
        Sequence<Tree<StatementLabel>> children = this.rep.newSequenceOfTree();
        StatementLabel label = this.rep.disassemble(children);
        localS.rep = children.remove(0);
        this.createNewRep(); // clears this
        return label.condition;
    }

    @Override
    public final void assembleIfElse(Condition c, Statement s1, Statement s2) {
        assert c != null : "Violation of: c is not null";
        assert s1 != null : "Violation of: s1 is not null";
        assert s2 != null : "Violation of: s2 is not null";
        assert s1 instanceof Statement2 : "Violation of: s1 is a Statement2";
        assert s2 instanceof Statement2 : "Violation of: s2 is a Statement2";
        assert s1.kind() == Kind.BLOCK : "Violation of: [s1 is a BLOCK statement]";
        assert s2.kind() == Kind.BLOCK : "Violation of: [s2 is a BLOCK statement]";

        Statement2 localS1 = (Statement2) s1;
        Statement2 localS2 = (Statement2) s2;
        StatementLabel label = new StatementLabel(Kind.IF_ELSE, c);
        Sequence<Tree<StatementLabel>> children = this.rep.newSequenceOfTree();
        children.add(0, localS1.rep);
        children.add(1, localS2.rep);
        this.rep.assemble(label, children);
        localS1.createNewRep(); // clears s1
        localS2.createNewRep(); // clears s2   
    }

    @Override
    public final Condition disassembleIfElse(Statement s1, Statement s2) {
        assert s1 != null : "Violation of: s1 is not null";
        assert s2 != null : "Violation of: s1 is not null";
        assert s1 instanceof Statement2 : "Violation of: s1 is a Statement2";
        assert s2 instanceof Statement2 : "Violation of: s2 is a Statement2";
        assert this.kind() == Kind.IF_ELSE : "Violation of: [s is an IF_ELSE statement]";

        Statement2 localS1 = (Statement2) s1;
        Statement2 localS2 = (Statement2) s2;
        Sequence<Tree<StatementLabel>> children = this.rep.newSequenceOfTree();
        StatementLabel label = this.rep.disassemble(children);
        localS2.rep = children.remove(1);
        localS1.rep = children.remove(0);
        this.createNewRep(); // clears this
        return label.condition;
    }

    @Override
    public final void assembleWhile(Condition c, Statement s) {
        assert c != null : "Violation of: c is not null";
        assert s != null : "Violation of: s is not null";
        assert s instanceof Statement2 : "Violation of: s is a Statement2";
        assert s.kind() == Kind.BLOCK : "Violation of: [s is a BLOCK statement]";

        Statement2 localS = (Statement2) s;
        StatementLabel label = new StatementLabel(Kind.WHILE, c);
        Sequence<Tree<StatementLabel>> children = this.rep.newSequenceOfTree();
        children.add(0, localS.rep);
        this.rep.assemble(label, children);
        localS.createNewRep(); // clears s

    }

    @Override
    public final Condition disassembleWhile(Statement s) {
        assert s != null : "Violation of: s is not null";
        assert s instanceof Statement2 : "Violation of: s is a Statement2";
        assert this.kind() == Kind.WHILE : "Violation of: [s is a WHILE statement]";

        Statement2 localS = (Statement2) s;
        Sequence<Tree<StatementLabel>> children = this.rep.newSequenceOfTree();
        StatementLabel label = this.rep.disassemble(children);
        localS.rep = children.remove(0);
        this.createNewRep(); // clears this
        return label.condition;
    }

    @Override
    public final void assembleCall(String inst) {
        assert inst != null : "Violation of: inst is not null";
        assert Tokenizer.isIdentifier(inst) : ""
                + "Violation of: inst is a valid IDENTIFIER";

        StatementLabel label = new StatementLabel(Kind.CALL, inst);
        this.rep.assemble(label, this.rep.newSequenceOfTree());

    }

    @Override
    public final String disassembleCall() {
        assert this.kind() == Kind.CALL : "Violation of: [s is a CALL statement]";

        //This needs to be reviewed
        Sequence<Tree<StatementLabel>> temp = this.rep.newSequenceOfTree();
        StatementLabel label = this.rep.disassemble(temp);
        String result = label.instruction;
        label.instruction = null;
        label.kind = Kind.BLOCK;
        this.rep.assemble(label, temp);
        return result;

    }

}
