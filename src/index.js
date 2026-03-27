import * as THREE from 'three';
import * as CANNON from 'cannon-es';
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x1a1a2e);
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
camera.position.set(0, 5, 15);
camera.lookAt(0, 0, 0);
const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setSize(window.innerWidth, window.innerHeight);
renderer.shadowMap.enabled = true;
document.body.appendChild(renderer.domElement);
const ambientLight = new THREE.AmbientLight(0xffffff, 0.6);
scene.add(ambientLight);
const directionalLight = new THREE.DirectionalLight(0xffffff, 0.8);
directionalLight.position.set(10, 20, 10);
directionalLight.castShadow = true;
scene.add(directionalLight);
const world = new CANNON.World();
world.gravity.set(0, -9.82, 0);
world.defaultContactMaterial.friction = 0.3;
world.defaultContactMaterial.restitution = 0.7;
const tableWidth = 3;
const tableHeight = 0.2;
const tableLength = 8;
const tableGeometry = new THREE.BoxGeometry(tableWidth, tableHeight, tableLength);
const tableMaterial = new THREE.MeshStandardMaterial({color: 0x2d5016, metalness: 0.1, roughness: 0.8});
const tableMesh = new THREE.Mesh(tableGeometry, tableMaterial);
tableMesh.castShadow = true;
tableMesh.receiveShadow = true;
scene.add(tableMesh);
const tableShape = new CANNON.Box(new CANNON.Vec3(tableWidth / 2, tableHeight / 2, tableLength / 2));
const tableBody = new CANNON.Body({ mass: 0, shape: tableShape });
tableBody.position.copy(tableMesh.position);
world.addBody(tableBody);
const ballRadius = 0.2;
const ballGeometry = new THREE.SphereGeometry(ballRadius, 32, 32);
const ballMaterial = new THREE.MeshStandardMaterial({color: 0xff0000, metalness: 0.9, roughness: 0.1});
const ballMesh = new THREE.Mesh(ballGeometry, ballMaterial);
ballMesh.position.set(0, 2, -2);
ballMesh.castShadow = true;
ballMesh.receiveShadow = true;
scene.add(ballMesh);
const ballShape = new CANNON.Sphere(ballRadius);
const ballBody = new CANNON.Body({mass: 1, shape: ballShape, linearDamping: 0.3, angularDamping: 0.3});
ballBody.position.copy(ballMesh.position);
world.addBody(ballBody);
const flipperLength = 1.5;
const flipperWidth = 0.2;
const flipperHeight = 0.1;
const leftFlipperGeometry = new THREE.BoxGeometry(flipperLength, flipperHeight, flipperWidth);
const flipperMaterial = new THREE.MeshStandardMaterial({color: 0xffff00, metalness: 0.4, roughness: 0.6});
const leftFlipperMesh = new THREE.Mesh(leftFlipperGeometry, flipperMaterial);
leftFlipperMesh.position.set(-0.8, 0.5, 3);
leftFlipperMesh.castShadow = true;
leftFlipperMesh.receiveShadow = true;
scene.add(leftFlipperMesh);
const leftFlipperShape = new CANNON.Box(new CANNON.Vec3(flipperLength / 2, flipperHeight / 2, flipperWidth / 2));
const leftFlipperBody = new CANNON.Body({mass: 0, shape: leftFlipperShape});
leftFlipperBody.position.copy(leftFlipperMesh.position);
world.addBody(leftFlipperBody);
const rightFlipperGeometry = new THREE.BoxGeometry(flipperLength, flipperHeight, flipperWidth);
const rightFlipperMesh = new THREE.Mesh(rightFlipperGeometry, flipperMaterial);
rightFlipperMesh.position.set(0.8, 0.5, 3);
rightFlipperMesh.castShadow = true;
rightFlipperMesh.receiveShadow = true;
scene.add(rightFlipperMesh);
const rightFlipperShape = new CANNON.Box(new CANNON.Vec3(flipperLength / 2, flipperHeight / 2, flipperWidth / 2));
const rightFlipperBody = new CANNON.Body({mass: 0, shape: rightFlipperShape});
rightFlipperBody.position.copy(rightFlipperMesh.position);
world.addBody(rightFlipperBody);
const keys = {};
window.addEventListener('keydown', (e) => { keys[e.key.toLowerCase()] = true; });
window.addEventListener('keyup', (e) => { keys[e.key.toLowerCase()] = false; });
const timeStep = 1 / 60;
function animate() {
    requestAnimationFrame(animate);
    world.step(timeStep);
    ballMesh.position.copy(ballBody.position);
    ballMesh.quaternion.copy(ballBody.quaternion);
    if (keys['z'] || keys['arrowleft']) {
        leftFlipperMesh.rotation.z = Math.PI / 6;
    } else {
        leftFlipperMesh.rotation.z = 0;
    }
    if (keys['m'] || keys['arrowright']) {
        rightFlipperMesh.rotation.z = -Math.PI / 6;
    } else {
        rightFlipperMesh.rotation.z = 0;
    }
    renderer.render(scene, camera);
}
animate();
window.addEventListener('resize', () => {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
});