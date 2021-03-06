class FootballerBrain_AI_MultiUseGoal extends FootballerBrain_AI_ManagedGoal {

	function hasGoal() { return mGoal != null; }
	function getGoal() { return mGoal; }
	
	function stop(brain) {
	
		if (mGoal != null) {
			
			mGoal.stop();
			mGoal = null;
		}
	}

	function update(brain, footballer, t, dt, goalClass, manager, goalSetupFunc, startSetupFunc) {
	
		if (mGoal == null) {
		
			mGoal = goalClass();
			
			if (goalSetupFunc != null)
				(goalSetupFunc.bindenv(manager))(brain, footballer, t, dt, mGoal);
		} 
		
		if (mGoal != null) {
		
			local result = mGoal.update(brain, footballer, t, dt, manager, startSetupFunc);
				
			if (result != FootballerBrain_AI_Goal.Result_Processing) {
			
				mGoal = null;
			}
			
			return result;
		}
		
		return FootballerBrain_AI_Goal.Result_None;
	}
	
	mGoal = null;
}