package test;

import adder.Hwj1;
import org.junit.After;
import org.junit.Before;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;
import tree.Tree;

import static org.junit.Assert.*;

public class Hwj1Test {

    @Before
    public void setUp() throws Exception {
    }

    @After
    public void tearDown() throws Exception {
    }

    @Test
    public void hwj1Create() {
        System.out.println("[T_HWJ1] Creazione HWJ1T object");
        Tree tree = new Tree();
        tree.generateBinaryTree(0);
        Hwj1 hwj1;

        hwj1 = new Hwj1("");

        assertNotNull(hwj1);
    }

    @Test
    public void testHwj1ComputeOnerousSumTreeDepth0() {
        System.out.println("[T_HWJ1] computeOnerousSumTree() su albero binario profondita' 0");
        int result;
        int depth = 0;
        Tree tree = new Tree();
        tree.generateBinaryTree(depth);
        Hwj1 hwj1 = new Hwj1("");

        result = hwj1.computeOnerousSum(tree.getRoot());

        assertEquals(1 , result); //(2^(depth+1))-1
    }

    @Test
    public void testHwj1ComputeOnerousSumTreeDepth1() {
        System.out.println("[T_HWJ1] computeOnerousSumTree() su albero binario profondita' 1");
        int result;
        int depth = 1;
        Tree tree = new Tree();
        tree.generateBinaryTree(depth);
        Hwj1 hwj1 = new Hwj1("");

        result = hwj1.computeOnerousSum(tree.getRoot());

        assertEquals(3 , result); //(2^(depth+1))-1
    }

    @Test
    public void testHwj1ComputeOnerousSumTreeDepth2() {
        System.out.println("[T_HWJ1] computeOnerousSumTree() su albero binario profondita' 2");
        int result;
        int depth = 2;
        Tree tree = new Tree();
        tree.generateBinaryTree(depth);
        Hwj1 hwj1 = new Hwj1("");

        result = hwj1.computeOnerousSum(tree.getRoot());

        assertEquals(7 , result); //(2^(depth+1))-1
    }

    @Test
    public void testHwj1ComputeOnerousSumTreeDepth10() {
        System.out.println("[T_HWJ1] computeOnerousSumTree() su albero binario profondita' 10");
        int result;
        int depth = 10;
        Tree tree = new Tree();
        tree.generateBinaryTree(depth);
        Hwj1 hwj1 = new Hwj1("");

        result = hwj1.computeOnerousSum(tree.getRoot());

        assertEquals(2047 , result); //(2^(depth+1))-1
    }

    @Test
    public void testHwj1ComputeOnerousSumTreeDepth15() {
        System.out.println("[T_HWJ1] computeOnerousSumTree() su albero binario profondita' 15");
        int result;
        int depth = 15;
        Tree tree = new Tree();
        tree.generateBinaryTree(depth);
        Hwj1 hwj1 = new Hwj1("");

        result = hwj1.computeOnerousSum(tree.getRoot());

        assertEquals(65535 , result); //(2^(depth+1))-1
    }
}