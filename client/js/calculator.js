function calc(){
	let polynomial=document.getElementById("input").value;
	if(polynomial[0]==='?'){
		alert("多项式不合法！");
		return;
	}
	let polynomialForShow=polynomial;
	let mode=document.getElementById("select").value;
	polynomial=mode+polynomial;
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
			document.getElementById("result").innerHTML=polynomialForShow+" = "+data;
	}).catch(function(err){
		console.log(err);
	});
}
