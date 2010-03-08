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
depth = -20
n = 600
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
plane:scale(50, 15, 100)
scene:add_child(plane)
plane:set_material(grass)

-- set up cow
cow_poly = gr.mesh('cow', readobj('cow.obj'))
factor = 2.0/(2.76+3.637)

cow_poly:set_material(hide)
factor = 2.0/(2.76+3.637)

cow_poly:set_material(hide)

cow_poly:translate(0.0, -1.0, 0.0)
cow_poly:scale(factor, factor, factor)
cow_poly:translate(0.0, 3.637, 0.0)

cow_number = 1
for j = 1, 9 do
	for k = 1, 4 do
		cow_instance = gr.node('cow' .. tostring(cow_number))
		scene:add_child(cow_instance)
		cow_instance:add_child(cow_poly)
		cow_instance:translate(-15 + j * 3, -13.7, - k * 5)
		cow_instance:rotate('Y', 90)
		cow_instance:scale(1.4, 1.4, 1.4)

		cow_number = cow_number + 1
	  end
end

--[[
for _, pt in pairs({
		      {{1,1.3-15,14}, 90},
		      {{5,1.3-15,-11}, 90},
		      {{-5.5,1.3-15,-3}, 90}}) do
   cow_instance = gr.node('cow' .. tostring(cow_number))
   scene:add_child(cow_instance)
   cow_instance:add_child(cow_poly)
   cow_instance:translate(unpack(pt[1]))
   cow_instance:rotate('Y', pt[2])
   cow_instance:scale(1.4, 1.4, 1.4)
   
   cow_number = cow_number + 1
end


cow_instance = gr.node('cow' .. tostring(cow_number))
scene:add_child(cow_instance)
cow_instance:add_child(cow_poly)
cow_instance:scale(0.5, 0.5, 0.5)
cow_instance:translate(0, -15, 20)
cow_instance:rotate('Y', 70)
--cow_instance:scale(0.5, 0.5, 0.5)

gr.render(scene,
	  'sierpinski.png', 256, 256,
	  {0, 0, 30}, {0, 0, -50}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({100, 100, 400}, {0.8, 0.8, 0.8}, {1, 0, 0})})
--]]
gr.render(scene,
	  'macho_cows.png', 32, 32,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0})})