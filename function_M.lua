--[[

役立ちそうな関数集です。
このファイルが必要なスクリプトがあります。

]]


--テツさんの格子線の関数
function Rot_x(x,y,z,angle)
 local x2,y2,z2
 x2=x
 y2=y*math.cos(angle)-z*math.sin(angle)
 z2=y*math.sin(angle)+z*math.cos(angle)
 return x2,y2,z2
end

function Rot_y(x,y,z,angle)
 local x2,y2,z2
 x2=x*math.cos(angle)+z*math.sin(angle)
 y2=y
 z2=-x*math.sin(angle)+z*math.cos(angle)
 return x2,y2,z2
end

function Rot_z(x,y,z,angle)
 local x2,y2,z2
 x2=x*math.cos(angle)-y*math.sin(angle)
 y2=x*math.sin(angle)+y*math.cos(angle)
 z2=z
 return x2,y2,z2
end

function Rotate(pos,RX,RY,RZ)
 local pos_num=#pos/3
 for i=1,pos_num do
   pos[3*i-2],pos[3*i-1],pos[3*i]=Rot_z(pos[3*i-2],pos[3*i-1],pos[3*i],math.rad(RZ))
   pos[3*i-2],pos[3*i-1],pos[3*i]=Rot_y(pos[3*i-2],pos[3*i-1],pos[3*i],math.rad(RY))
   pos[3*i-2],pos[3*i-1],pos[3*i]=Rot_x(pos[3*i-2],pos[3*i-1],pos[3*i],math.rad(RX))
 end
 return pos
end


--93さんのDelayMoveのコードを改変して関数化
function DelayMove_M(ind,posnum,r,seed,delaytime)

	local delay={}				--初期化

	if(ind==1)then				--0~1に値を収める
		for i=0,posnum-1 do
			delay[i+1]=i/(posnum-1)
		end
	elseif(ind==2)then
		for i=0,posnum-1 do
			delay[i+1]=(posnum-1-i)/(posnum-1)
		end
	elseif(ind==3)then
		for i=0,posnum-1 do
			delay[i+1]=math.abs((posnum-1)/2-i)/(posnum-1)
		end
	elseif(ind==4)then
		for i=0,posnum-1 do
			delay[i+1]=((posnum-1)/2-math.abs((posnum-1)/2-i))/(posnum-1)
		end
	else
		delay=ind
		if(#delay<posnum)then
			local hash=#delay
			for i=0,posnum-hash-1 do
				delay[hash+i+1]=delay[i+1]
			end
		end
	end

	for i=0,posnum-1 do			--randの計算
		local ra=obj.rand(0,1000,seed,15+i)/1000
		delay[i+1]=delay[i+1]+r*(ra-delay[i+1])
	end

	for i=0,posnum-1 do			--getvalue用の時間を求める
		delay[i+1]=delay[i+1]*delaytime*0.01
		delay[i+1]=math.min(obj.totaltime,math.max(0,obj.time-delay[i+1]))
	end

	return delay

end


--posとpの距離を求める関数
function distance_M(pos,p)

	local k={}

	for i=0,#pos/3-1 do
		local w,q,s
		w=p[1]-pos[i*3+1]
		q=p[2]-pos[i*3+2]
		s=p[3]-pos[i*3+3]
		k[i+1]=math.sqrt(w*w+q*q+s*s)
	end

	return k

end


--posとpからdelayを求める関数
function DelaiyField_M(pos,p)

	local k={}
	k=distance_M(pos,p)

	local mx,mn=1,1
	local posnum=#pos/3
	for i=0,posnum-2 do
		if(k[mx]<k[i+2])then mx=i+2 end end
	for i=0,posnum-2 do
		if(k[mn]>k[i+2])then mn=i+2 end end

	mx,mn=k[mx],k[mn]

	local ef={}
	for i=0,posnum-1 do
		ef[i+1]=(k[i+1]-mn)/mx
	end

	return ef

end

