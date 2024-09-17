function calc(){
	let polynomial=document.getElementById("input").value;
	var url="http://localhost:8080/calc";
	fetch(url,{
		method:"POST",
		headers:{
			"Content-Type":"text/plain",
		},
		mode:"cors",
		body:polynomial
	}).then(function(response){
		return response.text();
	}).then(function(data){
		if(data=="invalid")
			alert("多项式不合法！");
		else
			document.getElementById("result").innerHTML=polynomial+" = "+data;
	}).catch(function(err){
		console.log(err);
	});
}
