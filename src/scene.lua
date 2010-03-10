-- Final scene
-- This scene will draw an army of cows advancing to steal the previous
-- Sierpinski triforce of power knowledge and courage
-- However the cows must get past white cow Gandalf.
-- Title of this artwork "You shall not pass"

-- Approximately renders a 3d sierpinski triangle 

require('readobj')

pink = gr.material({1.0, 0.753, 0.796}, {0.0, 0.0, 0.0}, 0)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)


scene = gr.node('scene')
scene:rotate('X', 23)

yMax = 10
yMin = -10
xMax = 10
xMin = -10
zMin = -30
zMax = -10
depth = -30
n = 500
-- Corner 1
sphere = gr.sphere('corner1')
sphere:translate(0, yMax, depth)
sphere:set_material(pink)
scene:add_child(sphere)

-- Corner 2
sphere1 = gr.sphere('corner2')
sphere1:translate(xMin, yMin, depth)
sphere1:set_material(pink)
scene:add_child(sphere1)

-- Corner 3
sphere2 = gr.sphere('corner3')
sphere2:translate(xMax, yMin, depth)
sphere2:set_material(pink)
scene:add_child(sphere2)

-- Corner 4
sphere4 = gr.sphere('corner4')
sphere4:translate(0, yMin, zMin)
sphere4:set_material(pink)
--scene:add_child(sphere4)




-- Seed for random numbers
math.randomseed( os.time() )

-- Randomly start somewhere
y = math.random(yMin, yMax)
if y > 0 then
	x = math.random (-y, y)
	z = math.random (-y, y)
else
	x = math.random(y, -y)
	z = math.random(y, -y)
end

print ("start (" .. x .. ", " .. y .. ")")
for i=1, n do
	vertex = math.random(1, 3)

	if vertex == 1 then
		x = x / 2
		y = yMax + y
		y = y / 2
		z = (zMax + 3*z) /  6
	elseif vertex == 2 then
		x = xMin + x
		x = x / 2
		y = yMin + y
		y = y / 2
		z = zMax + z
		z = z / 2
	else
		x = xMax + x
		x = x / 2
		y = yMin + y
		y = y / 2
		z = zMax + z
		z = z / 2
	end

	print ("final (" .. x .. ", " .. y .. ", " .. z .. ")")
	sphere3 = gr.nh_sphere('s2' .. tostring(i), {x, y, depth}, 0.2)
--	sphere3:translate(x, y, 0)

	col = gr.material({x/20 + 0.5, y/20 + 0.5, 0}, {0.3, 0.3, 0.3}, 20)
	sphere3:set_material(col)
--	sphere3:scale(0.5, 0.5, 0.5)
	scene:add_child(sphere3)
end

plane = gr.mesh('plane', {
		   { -1, -1, -1 }, 
		   {  1, -1, -1 }, 
		   {  1, -1, 1 }, 
		   { -1, -1, 1  }
		}, {
		   {3, 2, 1, 0}
		})
plane:scale(50, 20, 100)
scene:add_child(plane)
plane:set_material(grass)

-- set up cow
cowMat = {}
for j = 1, 9 do
	cowMat[j] = {}
	for k = 1, 4 do
		cowMat[j][k] = gr.mesh('cow', readobj('cow.obj'))
		factor = 2.0/(2.76+3.637)

		cowMat[j][k]:set_material(hide)
		factor = 2.0/(2.76+3.637)

		cowMat[j][k]:set_material(hide)

		cowMat[j][k]:translate(0.0, -1.0, 0.0)
		cowMat[j][k]:scale(factor, factor, factor)
		cowMat[j][k]:translate(0.0, 3.637, 0.0)
	end
end
		
cow_poly = gr.mesh('cow', readobj('cow.obj'))
factor = 2.0/(2.76+3.637)

cow_poly:set_material(hide)
factor = 2.0/(2.76+3.637)

cow_poly:set_material(hide)

cow_poly:translate(0.0, -1.0, 0.0)
cow_poly:scale(factor, factor, factor)
cow_poly:translate(0.0, 3.637, 0.0)



-- Army of cows
cow_number = 1
colMat = {}
for j = 1, 7 do
	colMat[j] = {}
	for k = 1, 3 do
		colMat[j][k] = gr.material({j/9, 0, k/4}, {0.3, 0.3, 0.3}, 20)

		simSphere = gr.sphere('x' .. tostring(cow_number))	
		simSphere:translate(0.0, -1.0, 0.0)
		simSphere:scale(factor, factor, factor)
		simSphere:translate(0.0, 3.637, 0.0)
		simSphere:set_material(colMat[j][k])

		cowMat[j][k]:set_material(colMat[j][k])
		cow_instance = gr.node('cow' .. tostring(cow_number))
		scene:add_child(cow_instance)
		cow_instance:add_child(cowMat[j][k])
		cow_instance:translate(-13 + j * 3, -13.7, 5 - k * 5)
		cow_instance:rotate('Y', 90)
		cow_instance:scale(1.4, 1.4, 1.4)

		cow_number = cow_number + 1
	  end
end

-- Cow hero
whiteCol = gr.material({0.7, 0.788, 0.7}, {0.4, 0.4, 0.4}, 20)
cow_poly:set_material(whiteCol)
cowHero = gr.node('cow' .. tostring(cow_number))
--	chromechrome
scene:add_child(cowHero)
cowHero:add_child(cow_poly)
cowHero:translate(0, -17, -30)
cowHero:rotate('Y', 270)
cowHero:scale(5, 5, 5)

gr.render(scene,
	  'sierpinski.png', 512, 512,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0})})
	