<!DOCTYPE html>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
   <title>Random Maze Generator &amp; Solver</title>
</head>
<body>

<?php
$x = isset($_POST["x"]) ? htmlspecialchars($_POST["x"]) : 60;
$y = isset($_POST["y"]) ? htmlspecialchars($_POST["y"]) : 150;
$output = shell_exec("./mazegen.sh $x $y 2>&1");
?>

<br>
<img src="maze.png?<?php echo rand();//ensure referesh?>" alt="image"/>
<br><br>

<table border="0">
<tr><td>
<form action="index.php" method="post">
  X = <input type="number" name="x" autocomplete="off" value=<?php echo $x;?> >&nbsp;<input type="submit"> <br>
  Y = <input type="number" name="y" autocomplete="off" value=<?php echo $y;?> > <br>
</form>
</td>
<td>
  &uarr; new maze<br>
  &darr; maze solution using DFS
</td>
</tr></table>

<br>
<img src="maze.dfs.png?<?php echo rand();//ensure referesh?>" alt="image"/>
<br><br>


<p>
The display above shows the result of these steps:
</p><ol>
  <li>Mazegen: generates a random maze, start and goal cells, with dimensions specified.</li>
  <li>Solvemaze: finds a DFS solution path from start to goal.</li>
  <li>Printmaze: generates an encapsulated postscript file representing the maze problem and its solution.</li>
  <li>pstopnm: converts the postscript files to png for web display</li>
</ol>

<p>The maze generator starts with a maze with all walls "up" and follows the
  process: pick a wall at random; if the cells on either side are already
  connected, leave the wall standing, otherwise knock it down; stop the first
  time when start and goal are connected, and report the number of components;
  then continue until all cells are pairwise connected and verify that the maze
  has only one component. A Union-Find data structure is used to keep track of
  connectivity. The KISS pseudo-random number generator, invented by George
  Marsaglia, late Professor of Statistics at FSU, is used for random choices.
</p>

<p>The maze solver operates by converting the maze to its dual graph (which is incidentally a subgraph of the square lattice) and applying depth-first search on that graph.
</p>

<p>This demo is part of a project for my course COP4531 at FSU.</p>

<?php
echo '========= Diagnostic output from mazegen.sh =========<br>';
echo "<pre>$output</pre>";
?>

</body>
</html>

<!-- <p>Thanks to Brett Whissel for the postscript converter 
and Kamen Pavlov for help with the php page.
</p> -->
